#define FILE_OFFSET_BITS 64
  
#include <unistd.h>  
#include <stdio.h>  
#include <stdlib.h>  
#include <sys/stat.h>  
#include <sys/types.h>  
#include <dirent.h>  
#include <string.h>  
#include <time.h>  
#include <signal.h>  
#include <sys/time.h>  
#include <stdarg.h>  
#include <errno.h> 
#include "usb_copy.h"
#include "lib/libstorage.h"

#include "copythread.h"
  
/* 
* �����ļ�������ʾ���� 
*  
* ����˼·:�����ļ�һ�Σ����ļ�����¼��һ���б���������ֱ�Ӵ��б��еõ��ļ��� 
* ���߱������飬��һ��ͳ�ƣ��ڶ���ִ�� 
*  
* ���ڽ�����  
* 1. �ö�ʱ��ÿ��1��ˢ��һ��Ҫע�⺯����������� 
* 2. �����̹߳����߳�ͳ��/�������߳�ˢ��״̬���ƺ�С������� 
* 3. һ���߳��б仯ʱˢ�£��������޷���ʵ���� 
* 
* TODO 
* 2013-10-22 
* 1. ���������ѡ��Ĵ��� 
* 2. ����ļ��޷�����/Ŀ¼�޷����������ļ�/Ŀ¼�Ѿ����ڵ�����Ĵ��� 
* 3. ���û���κ��ļ��ɹ�����ʱ����ʾ��ϢBUG�������ļ�detected������£��� 
* 4. �����ļ���Ŀ�����Ѿ����ڵ�Ŀ¼��ʱ�Զ�����ļ���������ֱ�Ӹ��ơ� 
* 5. ����ʱ��human_time ����ʾ��ȥ��ʱ�䡣 
*  
* 2013-10-23 
* 1. ͳ�ƽ׶�ҲҪ��ʾ���� 
*  
* 2013-10-24 
* 1. overwrite ��ʾ��ȴ��û�����Ͷ�ʱ����ͻ������ 
*  
* 2013-10-29 
* 1. ��Դ����������������ѭ������Ҫ�����ļ������ж��Ƿ���ڵ�. 
* 
*/  

/*���ô��ļ�֧��*/  
//#define _LARGEFILE64_SOURCE  
//#define _FILE_OFFSET_BITS 64  
  
//#ifdef _LARGEFILE64_SOURCE  
//#define stat stat64  
//#define fopen fopen64  
//#define fread fread64  
//#define fwrite fwrite64  
//#endif  

/* ȫ�ֱ��� */  
int CopyThread::dir_is_empty(char* dirname)
{
    struct dirent *ent;
    DIR *dir = opendir(dirname);

    if(dir == nullptr)
    {
        //printf("open %s failed\n", dirname);
        return TRUE;
    }

    while((ent = readdir(dir)))
    {
        if((strcmp(".", ent->d_name) == 0) || (strcmp("..", ent->d_name) == 0))
        {
            continue;
        }

        return FALSE;
    }

    closedir(dir);

    return TRUE;
}
  
/* human readable time */  
char* CopyThread::human_time(time_t t, char *text)
{  
    int h,m,s;  
    h = static_cast<int>(t / HOUR);
    m = static_cast<int>((t % HOUR) / MINUTE);
    s = static_cast<int>(t % HOUR % MINUTE);
  
    if(h > 0)  
    {  
        sprintf(text, "%dh %dm %ds", h, m, s);  
    }  
    else if(m > 0)  
    {  
        sprintf(text, "%dm %ds", m, s);  
    }  
    else  
    {  
        sprintf(text, "%ds", s);  
    }  
    return text;  
}  
  

/*����Ŀ¼�ַ���,��Ҫ����ĩβ/������,frt snd ������������ͬʱΪ������û������*/  
char* CopyThread::make_path(char *dest, const char *frt, const char *snd)
{  
    if(nullptr == frt || strlen(frt) == 0)
    {  
        sprintf(dest, "%s", snd);  
    }  
    else if(nullptr == snd || strlen(snd) == 0)
    {  
        sprintf(dest, "%s", frt);  
    }  
    else  
    {  
        if(frt[strlen(frt) - 1] == '/')  
        {  
            sprintf(dest, "%s%s", frt, snd);  
        }  
        else  
        {  
            sprintf(dest, "%s/%s", frt, snd);  
        }  
    }  
    return dest;  
}  

/*  
* �������� 
* ��������ֻ��֤Դ�ļ�/�ļ��е�ÿһ�����һ��opp���� 
* ��opp�����ķ���ֵ�����Ƿ����ɨ�� 
* ���á����ա�ʽ������ 
* ֻ��һ������·���ֵΪFALSE��opp ��������OPP_CANCEL 
*/  
int CopyThread::walk_sum(const char* path_from, const char* path_to, const char* path_tree)
{  
    struct stat st;  
    DIR* dir = nullptr;
    struct dirent *entry = nullptr;
    char path_tree_new[MAX_PATH_LENGTH];
    char path_from_full[MAX_PATH_LENGTH];
    int ret_val = OPP_CONTINUE;
  
    /*���Դ������*/
    make_path(path_from_full, path_from, path_tree);
    if(-1 == stat(path_from_full, &st))
    {  
        //print_message(MSGT_ERROR, "can't access \"%s\".\n", path_from_full);
        return OPP_SKIP;  
    }  
  
    /*����һ�δ�����������ǰ��*/  
    if((ret_val = sum_up(path_tree, &st)) != OPP_CONTINUE)
    {  
        return ret_val;  
    }  
              
    /*�����Ŀ¼�������Ŀ¼���������*/  
    if(!S_ISDIR(st.st_mode))  
    {  
        return OPP_CONTINUE;  
    }  
  
    /*��Ŀ¼*/  
    if(!(dir = opendir(path_from_full)))  
    {  
       // print_message(MSGT_ERROR, "can't open directory \"%s\".\n", path_from_full);
        return OPP_SKIP;  
    }  
  
    /*���Ŀ¼*/  
    while((entry = readdir(dir)) != nullptr)
    {  
        /*����path_tree_new*/
        make_path(path_tree_new, path_tree, entry->d_name);  
        make_path(path_from_full, path_from, path_tree_new);  
      
        /*�޷����� skip*/  
        if(-1 == stat(path_from_full, &st))  
        {  
            qDebug("skip, can't access %s\"\".\n", path_from_full);
            continue;  
        }
        /* ���� . �� .. */
        if(S_ISDIR(st.st_mode) && (strcmp(".", entry->d_name) == 0 || strcmp("..", entry->d_name) == 0))  
        {  
            continue;  
        }  

        if(S_ISDIR(st.st_mode))  
        {  
          /*�ݹ鴦����Ŀ¼*/  
            if(walk_sum(path_from, path_to, path_tree_new) == OPP_CANCEL)
            {  
                ret_val = OPP_CANCEL;  
                break;  
            }  
        }  
        else  
        {  
            /*����������һ������*/  
            if(sum_up(path_tree_new, &st) == OPP_CANCEL)
            {
                ret_val = OPP_CANCEL;  
                break;  
            }  
        }  
    }  
    closedir(dir);  
    return ret_val;  
}  
  
/* ͳ�ƺ��� */  
int CopyThread::sum_up(const char* path_tree, const struct stat* st)
{  
    if(S_ISREG(st->st_mode))  
    {  
        sum.file++;
        sum.size += st->st_size;
    }  
    else if(S_ISDIR(st->st_mode))  
    {  
        sum.dir++;
    }  
    else  
    {  
        qWarning("skip:%s\n", path_tree);
    }  
	
    return OPP_CONTINUE;  
}  
int CopyThread::walk_copy(const char* path_from, const char* path_to, const char* path_tree)
{
    struct stat st;
    DIR* dir = nullptr;
    struct dirent *entry = nullptr;
    char path_tree_new[MAX_PATH_LENGTH];
    char path_from_full[MAX_PATH_LENGTH];
    int ret_val = OPP_CONTINUE;

    /*���Դ������*/
    make_path(path_from_full, path_from, path_tree);
    if(-1 == stat(path_from_full, &st))
    {
        qCritical("can't access \"%s\".\n", path_from_full);
        return OPP_SKIP;
    }

    /*����һ�δ�����������ǰ��*/
    if((ret_val = action(path_from, path_to, path_tree, &st)) != OPP_CONTINUE)
    {
        return ret_val;
    }

    /*�����Ŀ¼�������Ŀ¼���������*/
    if(!S_ISDIR(st.st_mode))
    {
        return OPP_CONTINUE;
    }

    /*��Ŀ¼*/
    if(!(dir = opendir(path_from_full)))
    {
        qCritical("can't open directory \"%s\".\n", path_from_full);
        return OPP_SKIP;
    }

    /*���Ŀ¼*/
    while((entry = readdir(dir)) != nullptr)
    {
        /*����path_tree_new*/
        make_path(path_tree_new, path_tree, entry->d_name);
        make_path(path_from_full, path_from, path_tree_new);

        /*�޷����� skip*/
        if(-1 == stat(path_from_full, &st))
        {
            qCritical("skip, can't access %s\"\".\n", path_from_full);
            continue;
        }
        /* ���� . �� .. */
        if(S_ISDIR(st.st_mode) && (strcmp(".", entry->d_name) == 0 || strcmp("..", entry->d_name) == 0))
        {
            continue;
        }

        if(S_ISDIR(st.st_mode))
        {
          /*�ݹ鴦����Ŀ¼*/
            if(walk_copy(path_from, path_to, path_tree_new) == OPP_CANCEL)
            {
                ret_val = OPP_CANCEL;
                break;
            }
        }
        else
        {
            /*����������һ������*/
            if(action(path_from, path_to, path_tree_new, &st) == OPP_CANCEL)
            {
                ret_val = OPP_CANCEL;
                break;
            }
        }
    }
    closedir(dir);
    return ret_val;
}
/* ���� */  
int CopyThread::action(const char* path_from, const char* path_to, const char* path_tree, const struct stat* st)
{
    int ret_val = OPP_CONTINUE;  
    char path_from_full[MAX_PATH_LENGTH];  
    char path_to_full[MAX_PATH_LENGTH];  
    size_t rd, wr, swr;  
    char buf[COPY_BUF_SIZE];  
    FILE *src_file, *dest_file;   
    struct stat st_dest;  

    make_path(path_from_full, path_from, path_tree);
    make_path(path_to_full, path_to, path_tree);

    if(S_ISREG(st->st_mode))
    {  
		if(strcmp(path_from_full, path_to_full) == 0)  
        {  
            ret_val = OPP_SKIP;  
            qCritical("skip, \"%s\" and \"%s\" are the same.\n", path_from_full, path_to_full);
        }  
        else if((src_file = fopen(path_from_full, "rb")) != nullptr)
        {  
            do  
            {
                /* open target file for write */  
                if((dest_file = fopen(path_to_full, "wb")) != nullptr)
                {
                    while((rd = fread(buf, 1, COPY_BUF_SIZE, src_file)) > 0)  
                    {  
                        wr = 0;  
                        do  
                        {  
                            swr = fwrite(buf + wr, 1, rd - wr, dest_file);  
                            wr += swr;  
                        }  
                        while(swr > 0 && wr < rd);  
                        copied.size += rd;
                      
                        if(wr != rd)  
                        {  
                            /*ֻ�в����ļ�������Ҳ��Ϊ�ɹ���Ϊ�ļ�ϵͳ���Ѿ�������ļ��ļ�¼��*/  
                            qCritical("write file error %s.\n", path_to_full);
                            break;  
                        }  
                    }  
                    fclose(dest_file);  
                    chmod(path_to_full, st->st_mode);  
                    copied.file++;
                    emit(sendToUI(num, sum, copied, copy_start_time, false));
                }  
                else  
                {  
                    ret_val = OPP_SKIP;  
                    qCritical("skip, can't open target file \"%s\"\n", path_to_full);
                }  
            }while(0);

            fclose(src_file);  
        }  
        else  
        {  
            ret_val = OPP_SKIP;  
            qCritical("skip, can't open source file \"%s\"\n", path_from_full);
        }  
    }  
    else if(S_ISDIR(st->st_mode))  
    {  
        /* directories */  
        qInfo("mkdir \"%s\"\n", path_to_full);
        
        if(0 == stat(path_to_full, &st_dest))  
        {  
            /*path_to_full already exist*/  
            if(S_ISDIR(st_dest.st_mode))  
            {  
                copied.dir++;
            }  
            else  
            {  
                ret_val = OPP_SKIP;  
                qWarning("skip, \"%s\" exists and it's not a directory.\n", path_to_full);
            }  
        }  
        else  
        {  
            /*try to make a new directory*/  
            if(0 == mkdir(path_to_full, st->st_mode))  
            {  
                chmod(path_to_full, st->st_mode);  
                copied.dir++;
            }  
            else  
            {  
                ret_val = OPP_SKIP;  
                qCritical("skip, \"%s\" mkdir failed.\n", path_to_full);
            }  
        }  
    }  
    else  
    {  
        ret_val = OPP_SKIP;  
        qWarning("skip, \"%s\" is not a file nor directory.\n", path_to_full);
    }  
    return ret_val;
}
  
/*��ֹѭ�����ƣ���Ŀ���ļ�/�ļ��в��ܰ�����Դ�ļ�/�ļ�����*/  
bool CopyThread::is_self_copy(const char* src, const char* dest)
{  
    /*�ϸ������Ӧ���Ȱ�src��dest��ת��Ϊ����·��Ȼ���ڱȽϣ����� 
     *Linux�µ����·���Ƚ��鷳�� ~ ./ ../ ../../ ��... 
    */  
    char c;  
    const char* sub = strstr(dest, src);
  
    if(sub)  
    {  
        c = sub[strlen(src)];  
        return c == '\0' || c == '/' || src[strlen(src) - 1] == '/';  
    }  
    else  
    {  
        return FALSE;  
    }  
}  

/**/
void CopyThread::is_transcoding()
{
    struct stat st;
    DIR* dir = nullptr;
    struct dirent *entry = nullptr;
    uint count = 0, num = 0;

    if(-1 == stat(mountDir, &st))
    {
        //print_message(MSGT_ERROR, "can't access \"%s\".\n", path_from_full);
        return;
    }

    /*�����Ŀ¼�������Ŀ¼���������*/
    if(!S_ISDIR(st.st_mode))
    {
        return;
    }

    /*��Ŀ¼*/
    if(!(dir = opendir(mountDir)))
    {
       // print_message(MSGT_ERROR, "can't open directory \"%s\".\n", path_from_full);
        return;
    }

    /*���Ŀ¼*/
    while(count < 10)
    {
        entry = readdir(dir);
        if((strcmp(".", entry->d_name) == 0 || strcmp("..", entry->d_name) == 0))
            continue;

        if(NULL == strstr(entry->d_name, "hiv"))
        {
            num++;
        }

        count ++;
        transcodingFlag = true;
    }

    if(num > 5)
        transcodingFlag = false;

    closedir(dir);
    return;
}

void CopyThread::transcoding_sum()
{
    int iTypeNum = 0;

    TYPE_CHAN_MAP_T stTypeChanMap[8];

    RECORD_SEARCH_PARAM_T stSearchParam;
    int iType, iChan;

    void *pHandle = nullptr;
    RECORD_INFO_T stRecInfo;

    memset(&stTypeChanMap[0], 0, sizeof(TYPE_CHAN_MAP_T) * 8);
    memset(&stSearchParam, 0, sizeof(RECORD_SEARCH_PARAM_T));
    memset(&stRecInfo, 0, sizeof(RECORD_INFO_T));

    if(storage_init(mountDir) != 0)
    {
        qCritical("can not init storage\n");
        return;
    }

    iTypeNum = storage_type_chan_map_get(&stTypeChanMap[0], 8);
    if(iTypeNum < 0)
    {
        qCritical("storage_type_chan_map_get failed\n");
        return;
    }

    stSearchParam.uStartTime = 0;
    stSearchParam.uEndTime = -1;

    for(iType = 0; iType < iTypeNum; iType++)
    {
        for(iChan = (int)stTypeChanMap[iType].StartChan; iChan <= (int)stTypeChanMap[iType].EndChan; iChan++)
        {
            stSearchParam.iType = iType;
            stSearchParam.iChan = iChan;

            pHandle = storage_record_list_create(&stSearchParam);
            if(pHandle == NULL)
            {
                qCritical("storage_record_list_create failed\n");
                continue;
            }

            while(storage_record_list_dequeue(pHandle, &stRecInfo) == 0)
            {
                sum.file++;
                qDebug() << sum.file << stRecInfo.sName;
                sum.size += stRecInfo.uFileSize;
            }
        }

        storage_record_list_release(pHandle);
    }
    qDebug() <<"file " << sum.file;
}

void CopyThread::transcoding_copy(const char *path_to)
{
    int iTypeNum = 0;
    TYPE_CHAN_MAP_T stTypeChanMap[8];

    RECORD_SEARCH_PARAM_T stSearchParam;
    int iType, iChan;

    void *pHandle = nullptr;
    RECORD_INFO_T stRecInfo;

    DEV_INFO_T stDevInfo;
    void *pFileHandle = nullptr;

    file_name_t file_name;
    char outPutFileName[100] = {0};
    FILE *pOutPutFile;
    char fileName[100] = {0};
    unsigned char pFileBuf[1024 * 256];
    size_t rd, wr, swr;
    char *ret = nullptr;

    memset(&file_name, 0, sizeof(file_name_t));
    memset(&stTypeChanMap[0], 0, sizeof(TYPE_CHAN_MAP_T) * 8);
    memset(&stSearchParam, 0, sizeof(RECORD_SEARCH_PARAM_T));
    memset(&stRecInfo, 0, sizeof(RECORD_INFO_T));
    memset(&stDevInfo, 0, sizeof(DEV_INFO_T));

    iTypeNum = storage_type_chan_map_get(&stTypeChanMap[0], 8);
    if(iTypeNum < 0)
    {
        qCritical("storage_type_chan_map_get failed\n");
        return;
    }

    stSearchParam.uStartTime = 0;
    stSearchParam.uEndTime = -1;

    for(iType = 0; iType < iTypeNum; iType++)
    {
        for(iChan = (int)stTypeChanMap[iType].StartChan; iChan <= (int)stTypeChanMap[iType].EndChan; iChan++)
        {
            stSearchParam.iType = iType;
            stSearchParam.iChan = iChan;
            pHandle = storage_record_list_create(&stSearchParam);
            if(pHandle == NULL)
            {
                qCritical("storage_record_list_create failed\n");
                continue;
            }

            while(storage_record_list_dequeue(pHandle, &stRecInfo) == 0)
            {
                if(storage_record_dev_info_get(&stRecInfo.sName[0], &stDevInfo) != 0)
                {
                    qCritical("storage_record_dev_info_get failed:%s", stRecInfo.sName);
                    continue;
                }

                pFileHandle = storage_record_open(&stRecInfo.sName[0]);
                if(pFileHandle == NULL)
                {
                    qCritical("storage_record_open faile:%s", stRecInfo.sName);
                    continue;
                }

                memcpy(&file_name.sCarriageNum[0], &stDevInfo.sCarriageNum[0], strlen(stDevInfo.sCarriageNum));
                memcpy(&file_name.sTrainNum[0], &stDevInfo.sTrainNum[0], strlen(stDevInfo.sTrainNum));
                memcpy(&file_name.sDriverNum[0], &stDevInfo.sDriverNum[0], strlen(stDevInfo.sDriverNum));
                memcpy(&file_name.sYear[0], &stRecInfo.sName[5], 4);
                memcpy(&file_name.sMonth[0], &stRecInfo.sName[9], 2);
                memcpy(&file_name.sDay[0], &stRecInfo.sName[11], 2);
                memcpy(&file_name.sHour[0], &stRecInfo.sName[14], 2);
                memcpy(&file_name.sMinutes[0], &stRecInfo.sName[16], 2);
                memcpy(&file_name.sSeconds[0], &stRecInfo.sName[18], 2);

                if(strlen(file_name.sCarriageNum) == 0)
                {
                    if(strlen(file_name.sDriverNum) == 0)
                    {
                        sprintf(fileName, "%s_%s-%s-%s_%s-%s-%s", &file_name.sTrainNum[0], &file_name.sYear[0],
                            &file_name.sMonth[0], &file_name.sDay[0], &file_name.sHour[0], &file_name.sMinutes[0],
                            &file_name.sSeconds[0]);
                    }
                    else
                    {
                        sprintf(fileName, "%s_%s_%s-%s-%s_%s-%s-%s", &file_name.sTrainNum[0], &file_name.sDriverNum[0],&file_name.sYear[0],
                            &file_name.sMonth[0], &file_name.sDay[0], &file_name.sHour[0], &file_name.sMinutes[0],
                            &file_name.sSeconds[0]);
                    }
                }
                else
                {
                    if(strlen(file_name.sDriverNum) == 0)
                    {
                        sprintf(fileName, "%s_%s_%s-%s-%s_%s-%s-%s", &file_name.sCarriageNum[0],
                                &file_name.sTrainNum[0], &file_name.sYear[0],&file_name.sMonth[0],
                                &file_name.sDay[0], &file_name.sHour[0], &file_name.sMinutes[0],
                            &file_name.sSeconds[0]);
                    }
                    else
                    {
                        sprintf(fileName, "%s_%s_%s_%s-%s-%s_%s-%s-%s", &file_name.sCarriageNum[0],
                                &file_name.sTrainNum[0],&file_name.sDriverNum[0],&file_name.sYear[0],
                            &file_name.sMonth[0], &file_name.sDay[0], &file_name.sHour[0],
                                &file_name.sMinutes[0],&file_name.sSeconds[0]);
                    }
                }
                ret = strchr(&stRecInfo.sName[0], '.');

                sprintf(outPutFileName, "%s/%s%s", path_to, fileName, ret);
                qDebug() << "file" << outPutFileName;
                pOutPutFile = fopen(outPutFileName, "wb");
                if(pOutPutFile == NULL)
                {
                    qCritical("create output file failed, path:%s", outPutFileName);
                    storage_record_close(pFileHandle);
                    continue;
                }

                while((rd = storage_record_read(pFileHandle, pFileBuf, 1024*256)) > 0)
                {
                    wr = 0;
                    do
                    {
                        swr = fwrite(pFileBuf + wr, 1, rd - wr, pOutPutFile);
                        wr += swr;
                    }
                    while(swr > 0 && wr < rd);
                    copied.size += rd;

                    if(wr != rd)
                    {
                        /*ֻ�в����ļ�������Ҳ��Ϊ�ɹ���Ϊ�ļ�ϵͳ���Ѿ�������ļ��ļ�¼��*/
                        qCritical("write file error %s.\n", outPutFileName);
                        break;
                    }
                }
                qDebug() << "coped one";
                fclose(pOutPutFile);
                storage_record_close(pFileHandle);
                copied.file++;
                emit(sendToUI(num, sum, copied, copy_start_time, false));
            }
        }

        storage_record_list_release(pHandle);
    }
}

/*�������������α���*/  
int CopyThread::cp_task(char *dir)
{  
    struct stat st_src, st_dest;  
    char path_to_fixed[MAX_PATH_LENGTH];  
    char *path_from = nullptr, *path_to = nullptr, *file_name = nullptr;
    /* ��һ�α�����ͳ�� */  
    sum.file = 0;  
    sum.dir = 0;  
    sum.size = 0;  

    if((num >= 0) && (num <= 1))
    {
        path_to = path[0];
    }
    else if((num >= 2) && (num <= 3))
    {
        path_to = path[1];
    }
    else if((num >= 4) && (num <= 5))
    {
        path_to = path[2];
    }
    else if((num >= 6) && (num <= 7))
    {
        path_to = path[3];
    }
    else if((num >= 8) && (num <= 9))
    {
        path_to = path[4];
    }
    else if((num >= 10) && (num <= 11))
    {
        path_to = path[5];
    }
    else if((num >= 12) && (num <= 13))
    {
        path_to = path[6];
    }
    else if((num >= 14) && (num <= 15))
    {
        path_to = path[7];
    }

    path_from = dir;
    is_transcoding();

    time(&copy_start_time);
    sum.file = 0;
    sum.dir = 0;
    sum.size = 0;
    copied.file = 0;
    copied.dir = 0;
    copied.size = 0;

    if(transcodingFlag == true)
    {
        transcoding_sum();
        transcoding_copy(path_to);
    }
    else
    {
        walk_sum(path_from, path_to, nullptr);
        if(sum.file == 0 && sum.dir == 0)
        {
            qInfo("nothing found.\n");
        }
        else
        {
            path_from = dir;

            /*Դ�Ƿ����*/
            if(-1 == stat(path_from, &st_src))
            {
                qCritical("\"%s\" doesn't exist.\n", path_from);
                return 0;
            }

            /*
            * ���Դ���ļ�����Ŀ�����Ѿ����ڵ�Ŀ¼�����Զ������ļ���
            * ���Ŀ�����Ѿ����ڵ��ļ������ж��Ƿ�ָ��ͬһ���ļ� inode number
            */
            if(S_ISREG(st_src.st_mode))
            {
                if((0 == stat(path_to, &st_dest)) && S_ISDIR(st_dest.st_mode))
                {
                    file_name = strrchr(path_from, '/');
                    path_to = make_path(path_to_fixed, path_to, file_name ? file_name + 1 : path_from);
                }
            }
            else if(S_ISDIR(st_src.st_mode))
            {
                if(is_self_copy(path_from, path_to))
                {
                    /*Դ��Ŀ¼ʱҪ��ֹѭ������*/
                    qCritical("can't xcp \"%s\" -> \"%s\"\n", path_from, path_to);
                    return 0;
                }
            }
            else
            {
                qWarning("skip \"%s\" not a file nor a directory.\n", path_from);
                return 0;
            }

            walk_copy(path_from, path_to, nullptr);
        }
    }

    return 0;
}  

