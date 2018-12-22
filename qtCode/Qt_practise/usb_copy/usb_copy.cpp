#define FILE_OFFSET_BITS 64
  
#include <unistd.h>  
#include <stdio.h>  
#include <stdlib.h>  
#include <sys/stat.h>  
#include <sys/statfs.h>
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

    qDebug() << "star transcoding";

    qDebug() << mountDir;
    if(-1 == stat(mountDir, &st))
    {
        transcodingFlag = false;
        qCritical("transcoding dir can't get infomation");
        return;
    }

    /*�����Ŀ¼�������Ŀ¼���������*/
    if(!S_ISDIR(st.st_mode))
    {
        transcodingFlag = false;
        qCritical("transcoding is not a dir");
        return;
    }

    /*��Ŀ¼*/
    if(!(dir = opendir(mountDir)))
    {
        transcodingFlag = false;
        qCritical("transcoding dir can't open");
        return;
    }

    /*���Ŀ¼*/
    while((entry = readdir(dir)) != NULL)
    {
        if((strcmp(".", entry->d_name) == 0 || strcmp("..", entry->d_name) == 0))
            continue;

        if(NULL != strstr(entry->d_name, "hiv"))
        {
            qDebug() << "not hiv" << entry->d_name;
            num++;
        }

        if(num > 5)
        {
            transcodingFlag = true;
            break;
        }
    }

    qDebug() << "end transcoding";

    closedir(dir);
    return;
}

void CopyThread::transcoding_sum()
{
    int iTypeNum = 0;
    int iType, iChan;
    unsigned int iMediaNo = 0;
    void *pHandle = nullptr;
    RECORD_INFO_T stRecInfo;
    TYPE_CHAN_MAP_T stTypeChanMap[8];
    RECORD_SEARCH_PARAM_T stSearchParam;

    memset(&stTypeChanMap[0], 0, sizeof(TYPE_CHAN_MAP_T) * 8);
    memset(&stSearchParam, 0, sizeof(RECORD_SEARCH_PARAM_T));
    memset(&stRecInfo, 0, sizeof(RECORD_INFO_T));

    qDebug() << "star init";
    iMediaNo =storage_init(mountDir);
    qDebug() << iMediaNo << mountDir;
    if(iMediaNo < 0)
    {
        qCritical("can not init storage\n");
        return;
    }

    iTypeNum = storage_type_chan_map_get(iMediaNo, &stTypeChanMap[0], 8);
    if(iTypeNum < 0)
    {
        qCritical("storage_type_chan_map_get failed\n");
        return;
    }

    stSearchParam.uMediaNo = iMediaNo;
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
                sum.size += stRecInfo.uFileSize;
                memcpy(&file_name[file_num][0], &stRecInfo.sName[0], strlen(&stRecInfo.sName[0]));

                qDebug() << sum.file << file_name[file_num] << &stRecInfo.sName[0];
                file_num ++;
            }
        }

        storage_record_list_release(pHandle);
    }

    qDebug() <<"file " << sum.file;
}

#if 0
void CopyThread::transcoding_copy(const char *path_to)
{
    void *pFileHandle = nullptr;
    char outPutFileName[100] = {0};
    FILE *pOutPutFile = nullptr;
    //char path_full[100] = {0};
    unsigned char pFileBuf[4096];
    size_t rd, wr, swr;

    for(int j = 0; j < file_num; j++)
    {
      //  memset(path_full, 0, strlen(path_full));
       // sprintf(path_full, "/media/usb%d/%s",num, file_name[j]);
        qDebug() << "star open" << pFileHandle << currentThreadId();
        pFileHandle = storage_record_open(file_name[j]);
       // pFileHandle = fopen(path_full, "rb");
        if(pFileHandle == nullptr)
        {
            qCritical("storage_record_open faile:%s", file_name[j]);
            continue;
        }
        qDebug() << "end open" << pFileHandle << currentThreadId();

        //sprintf(outPutFileName, "%s/%s%s", secondOutPutDir, fileName, ret);
        sprintf(outPutFileName, "%s/%s", path_to, file_name[j]);

        qDebug() << "file" << outPutFileName << currentThreadId();
        pOutPutFile = fopen(outPutFileName, "wb");
        if(pOutPutFile == NULL)
        {
            qCritical("create output file failed, path:%s", outPutFileName);
            storage_record_close(pFileHandle);
            continue;
        }

       // pFileBuf = (unsigned char *)malloc(1024*256);
        qDebug() << "star copy" << currentThreadId();
        //copy_num_mutex.lock();
        while(1)
        {
            rd = storage_record_read(pFileHandle, pFileBuf, 4096);

            if(rd <= 0)
            {
                //fflush(pOutPutFile);
                fclose(pOutPutFile);
                storage_record_close(pFileHandle);
                break;
            }

          //  copy_num_mutex.lock();

            fwrite(pFileBuf, 1, rd, pOutPutFile);
         //   copy_num_mutex.unlock();

            copied.size += rd;
        }
        //copy_num_mutex.unlock();

//        free(pFileBuf);
       // qDebug() << "coped one";
        qDebug() << "end copy" << currentThreadId();
       // fclose(pOutPutFile);
       // storage_record_close(pFileHandle);
        copied.file++;
        emit(sendToUI(num, sum, copied, copy_start_time, false));
    }
}
#endif
#if 1
void CopyThread::transcoding_copy(const char *path_to)
{
    size_t rd, wr, swr;
    FILE *pOutPutFile;
    DEV_INFO_T stDevInfo;
    file_name_t file_new_name;
    char *ret = nullptr;
    void *pFileHandle = nullptr;
    unsigned char pFileBuf[COPY_BUF_SIZE] = {0};
    char fileName[100] = {0};
    char outPutFileName[100] = {0};
    char firstOutPutDir[100] = {0};
    char secondOutPutDir[100] = {0};

    memset(&stDevInfo, 0, sizeof(DEV_INFO_T));

    for(int file = 0; file < file_num; file++)
    {
        if(storage_record_dev_info_get(&file_name[file][0], &stDevInfo) != 0)
        {
            qCritical("storage_record_dev_info_get failed:%s", &file_name[file][0]);
            continue;
        }

        pFileHandle = storage_record_open(&file_name[file][0]);
        if(pFileHandle == NULL)
        {
            qCritical("storage_record_open faile:%s", &file_name[file][0]);
            continue;
        }

        memcpy(&file_new_name.sCarriageNum[0], &stDevInfo.sCarriageNum[0], strlen(stDevInfo.sCarriageNum));
        memcpy(&file_new_name.sTrainNum[0], &stDevInfo.sTrainNum[0], strlen(stDevInfo.sTrainNum));
        memcpy(&file_new_name.sDriverNum[0], &stDevInfo.sDriverNum[0], strlen(stDevInfo.sDriverNum));
        memcpy(&file_new_name.sYear[0], &file_name[file][5], 4);
        memcpy(&file_new_name.sMonth[0], &file_name[file][9], 2);
        memcpy(&file_new_name.sDay[0], &file_name[file][11], 2);
        memcpy(&file_new_name.sHour[0], &file_name[file][14], 2);
        memcpy(&file_new_name.sMinutes[0], &file_name[file][16], 2);
        memcpy(&file_new_name.sSeconds[0], &file_name[file][18], 2);

        if(strlen(file_new_name.sCarriageNum) == 0)
        {
            if(strlen(file_new_name.sDriverNum) == 0)
            {
                sprintf(fileName, "%s_%s-%s-%s_%s-%s-%s", &file_new_name.sTrainNum[0], &file_new_name.sYear[0],
                    &file_new_name.sMonth[0], &file_new_name.sDay[0], &file_new_name.sHour[0], &file_new_name.sMinutes[0],
                    &file_new_name.sSeconds[0]);
            }
            else
            {
                sprintf(fileName, "%s_%s_%s-%s-%s_%s-%s-%s", &file_new_name.sTrainNum[0], &file_new_name.sDriverNum[0],&file_new_name.sYear[0],
                    &file_new_name.sMonth[0], &file_new_name.sDay[0], &file_new_name.sHour[0], &file_new_name.sMinutes[0],
                    &file_new_name.sSeconds[0]);
            }
        }
        else
        {
            if(strlen(file_new_name.sDriverNum) == 0)
            {
                sprintf(fileName, "%s_%s_%s-%s-%s_%s-%s-%s", &file_new_name.sCarriageNum[0],
                        &file_new_name.sTrainNum[0], &file_new_name.sYear[0],&file_new_name.sMonth[0],
                        &file_new_name.sDay[0], &file_new_name.sHour[0], &file_new_name.sMinutes[0],
                    &file_new_name.sSeconds[0]);
            }
            else
            {
                sprintf(fileName, "%s_%s_%s_%s-%s-%s_%s-%s-%s", &file_new_name.sCarriageNum[0],
                        &file_new_name.sTrainNum[0],&file_new_name.sDriverNum[0],&file_new_name.sYear[0],
                    &file_new_name.sMonth[0], &file_new_name.sDay[0], &file_new_name.sHour[0],
                        &file_new_name.sMinutes[0],&file_new_name.sSeconds[0]);
            }
        }
        ret = strchr(&file_name[file][0], '.');

        sprintf(firstOutPutDir, "%s/%s_%s_%s", path_to, file_new_name.sCarriageNum, file_new_name.sTrainNum, file_new_name.sDriverNum);

        if(access(firstOutPutDir, NULL) != 0)
        {
            mkdir(firstOutPutDir, 0777);
        }

        sprintf(secondOutPutDir, "%s/%s%s%s", firstOutPutDir,file_new_name.sYear, file_new_name.sMonth, file_new_name.sDay);
        if(access(secondOutPutDir, NULL) != 0)
        {
            mkdir(secondOutPutDir, 0777);
        }

        sprintf(outPutFileName, "%s/%s%s", secondOutPutDir, fileName, ret);

        qDebug() << "file" << outPutFileName;
        pOutPutFile = fopen(outPutFileName, "wb");
        if(pOutPutFile == NULL)
        {
            qCritical("create output file failed, path:%s", outPutFileName);
            storage_record_close(pFileHandle);
            continue;
        }

        while((rd = storage_record_read(pFileHandle, pFileBuf, COPY_BUF_SIZE)) > 0)
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
#endif
int CopyThread::search_dir_to_write(char *dir, unsigned int i)
{
    unsigned int hard_disk_num = 0;
    unsigned int count = 0;
    char *write_dir = nullptr;

    hard_disk_num = i;
    write_dir = dir;

    /*search dir to write*/
    while(count < HARD_DISK_MAX_NUM)
    {
        if(is_dir_writting_num(hard_disk_num) == false)
        {
            if(is_dir_larger(write_dir) == true)
            {
                return hard_disk_num;
            }
        }

        count++;
        hard_disk_num++;
        if(hard_disk_num >= HARD_DISK_MAX_NUM)
        {
            hard_disk_num = 0;
        }

        write_dir = path[hard_disk_num];
    }

    hard_disk_num = i;
    count = 0;
    write_dir = dir;

    /*search dir to delete*/
    while(1)
    {
        if(is_dir_writting_num(hard_disk_num) == false)
        {
            dir_full_and_delete(write_dir, nullptr);

            if(is_dir_larger(write_dir))
            {
                return hard_disk_num;
            }
        }

        hard_disk_num++;
        if(hard_disk_num >= HARD_DISK_MAX_NUM)
        {
            hard_disk_num = 0;
        }

        write_dir = path[hard_disk_num];
    }

    return -1;
}

int CopyThread::delete_min_time_dir(char *dir_del)
{
    int date[1024] = {0};
    int num = 0, min = 0;
    struct stat st;
    DIR* dir = nullptr;
    char dir_time_del[10];
    struct dirent *entry = nullptr;
    char path_from_full[MAX_PATH_LENGTH];
    char dir_name[MAX_PATH_LENGTH];

    /*��Ŀ¼*/
    if(!(dir = opendir(dir_del)))
    {
        qCritical("can't get %s information function:%s", path_from_full, __FUNCTION__);
        return OPP_SKIP;
    }

    /*���Ŀ¼*/
    while((entry = readdir(dir)) != nullptr)
    {
        /* ���� . �� .. */
        if(strcmp(".", entry->d_name) == 0 || strcmp("..", entry->d_name) == 0)
        {
            continue;
        }

        date[num++] = atoi(entry->d_name);
    }

    min = date[0];
    for(int i = 1; i < num; i++)
    {
       if(date[i] < min)
           min = date[i];
    }

    sprintf(dir_time_del, "%d", min);
    make_path(path_from_full, dir_del, dir_time_del);

    // �������ݽ�����Ŀ¼�����ڣ�ֱ�ӷ���
    if (0 != access(path_from_full, F_OK))
    {
        qCritical("can't access:%s function:%s", path_from_full, __FUNCTION__);
        return OPP_SKIP;
    }
    // ��ȡĿ¼����ʧ�ܣ����ش���
    if(0 > stat(path_from_full, &st))
    {
        qCritical("get directory:%s stat error function:%s", path_from_full, __FUNCTION__);
        return OPP_SKIP;
    }

    if(S_ISREG(st.st_mode))
    {
        // ��ͨ�ļ�ֱ��ɾ��
        remove(path_from_full);
        return OPP_CONTINUE;
    }
    else if(S_ISDIR(st.st_mode))
    {
        dir = opendir(path_from_full);
        while((entry = readdir(dir)) != NULL)
        {
            // ���� . �� ..
            if(strcmp(".", entry->d_name) == 0 || strcmp("..", entry->d_name) == 0)
            {
                continue;
            }

            sprintf(dir_name, "%s/%s", path_from_full, entry->d_name);
            remove(dir_name);
        }

        closedir(dir);
        rmdir(path_from_full);
        // ɾ����Ŀ¼
    }
    else
    {
        qCritical("unknow file type!");
        return OPP_SKIP;
    }

    return OPP_CONTINUE;
}

void CopyThread::dir_full_and_delete(char *dir_from, char *dir_tree)
{
    struct stat st;
    DIR* dir = nullptr;
    struct dirent *entry = nullptr;
    char path_tree_new[MAX_PATH_LENGTH];
    char path_from_full[MAX_PATH_LENGTH];

    /*���Դ������*/
    make_path(path_from_full, dir_from, dir_tree);
    if(-1 == stat(path_from_full, &st))
    {
        qCritical("can't get %s information function:%s", path_from_full, __FUNCTION__);
        return;
    }

    /*�����Ŀ¼�������Ŀ¼���������*/
    if(!S_ISDIR(st.st_mode))
    {
        qCritical("must delete dir function:%s", __FUNCTION__);
        return;
    }

    /*��Ŀ¼*/
    if(!(dir = opendir(path_from_full)))
    {
        qCritical("can't open dir:%s function:%s", path_from_full, __FUNCTION__);
        return;
    }

    /*���Ŀ¼*/
    while((entry = readdir(dir)) != nullptr)
    {
        /*����path_tree_new*/
        make_path(path_tree_new, dir_tree, entry->d_name);
        make_path(path_from_full, dir_from, path_tree_new);

        /*�޷����� skip*/
        if(-1 == stat(path_from_full, &st))
        {
            qDebug("skip, can't access %s function:%s\"\".\n", path_from_full, __FUNCTION__);
            continue;
        }
        /* ���� . �� .. */
        if(S_ISDIR(st.st_mode) && (strcmp(".", entry->d_name) == 0 || strcmp("..", entry->d_name) == 0))
        {
            continue;
        }

        if(S_ISDIR(st.st_mode))
        {
            if(delete_min_time_dir(path_from_full) == OPP_CONTINUE)
            {
               return;
            }
        }
        else
        {
            qCritical("is not a dir");
            continue;
        }
    }

    closedir(dir);
    return;
}

bool CopyThread::is_dir_writting_num(unsigned int i)
{
    if(dir_writting_num[i] >= 2)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool CopyThread::is_dir_larger(char *dir)
{
    unsigned long long dir_size = 0;
    struct statfs s;

    memset(&s, 0, sizeof(struct statfs));
    if(0 != statfs(dir, &s))
    {
        return false;
    }
    else
    {
        dir_size = s.f_bfree * s.f_bsize;
        qDebug() << "dir_size" << dir_size;
        if(dir_size > usb_size)
            return true;
    }

    return false;
}

/*�������������α���*/  
int CopyThread::cp_task(char *dir)
{  
    int i = 0;
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
        i = 0;
    }
    else if((num >= 2) && (num <= 3))
    {
        path_to = path[1];
        i = 1;
    }
    else if((num >= 4) && (num <= 5))
    {
        path_to = path[2];
        i = 2;
    }
    else if((num >= 6) && (num <= 7))
    {
        path_to = path[3];
        i = 3;
    }
    else if((num >= 8) && (num <= 9))
    {
        path_to = path[4];
        i = 4;
    }
    else if((num >= 10) && (num <= 11))
    {
        path_to = path[5];
        i = 5;
    }
    else if((num >= 12) && (num <= 13))
    {
        path_to = path[6];
        i = 6;
    }
    else if((num >= 14) && (num <= 15))
    {
        path_to = path[7];
        i = 7;
    }
    qDebug() << "star search";

    /**/
    i = search_dir_to_write(path_to, i);
    if(i < 0)
    {
        qCritical("can't find dir to write");
        return 0;
    }
    else
    {
        copy_num_mutex.lock();
        dir_writting_num[i]++;
        path_to = path[i];
        copy_num_mutex.unlock();
    }

    qDebug() << "end search";

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
        if(sum.file == 0 && sum.dir == 0)
        {
            sendUDevInfoTranscoding(num, sum.size);
            qInfo("transcoding sum nothing found.\n");
            return -1;
        }
        sendUDevInfoTranscoding(num, sum.size);

        transcoding_copy(path_to);

        emit(sendToUI(num, sum, copied, copy_start_time, true));
        storage_clear(mountDir);
    }
    else
    {
        walk_sum(path_from, path_to, nullptr);
        if(sum.file == 0 && sum.dir == 0)
        {
            qInfo("nothing found.\n");
            return -1;
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
    copy_num_mutex.lock();
    dir_writting_num[i]--;
    copy_num_mutex.unlock();

    return 0;
}  

