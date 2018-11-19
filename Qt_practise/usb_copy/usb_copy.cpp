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
        printf("open %s failed\n", dirname);
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
  
/* 
* �����״̬����Ȼ���������Ϣ 
* 1. ״̬���������ڵ�ǰ����������� 
* 2. printerrorֻ����״̬���ֱ���ʾ֮�����������ʱ������װ֮��ʹ�á� 
*/  
void CopyThread::print_message(int t, const char* fmt, ...)
{  
    char real_fmt[MAX_FMTSTR_LENGTH];  
    va_list args;  
    va_start(args, fmt);  
  
    if(t == MSGT_WARNING || t == MSGT_ERROR)  
    {  
        /*quiet, don't output warning nor error message*/  
    }  
    else  
    {  
        sprintf(real_fmt, "\r\033[K%s", fmt);  
        vprintf(real_fmt, args);  
    }  
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
        print_message(MSGT_ERROR, "can't access \"%s\".\n", path_from_full);  
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
        print_message(MSGT_ERROR, "can't open directory \"%s\".\n", path_from_full);  
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
        print_message(MSGT_WARNING, "skip:%s\n", path_tree);
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
        print_message(MSGT_ERROR, "can't access \"%s\".\n", path_from_full);
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
        print_message(MSGT_ERROR, "can't open directory \"%s\".\n", path_from_full);
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
            //qDebug("skip, can't access %s\"\".\n", path_from_full);
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
        //copy->print_message(MSGT_VERBOSE, "cp \"%s\" -> \"%s\".\n", path_from_full, path_to_full);
       // qDebug("cp \"%s\" -> \"%s\"", path_from_full, path_to_full);
       // qDebug("copy->num:%d", num);
		if(strcmp(path_from_full, path_to_full) == 0)  
        {  
            ret_val = OPP_SKIP;  
            print_message(MSGT_ERROR, "skip, \"%s\" and \"%s\" are the same.\n", path_from_full, path_to_full);
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
                            qDebug("write file error %s.\n", path_to_full);
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
                    qDebug("skip, can't open target file \"%s\"\n", path_to_full);
                }  
            }while(0);

            fclose(src_file);  
        }  
        else  
        {  
            ret_val = OPP_SKIP;  
            print_message(MSGT_ERROR, "skip, can't open source file \"%s\"\n", path_from_full);
        }  
    }  
    else if(S_ISDIR(st->st_mode))  
    {  
        /* directories */  
        print_message(MSGT_VERBOSE, "mkdir \"%s\"\n", path_to_full);
        
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
                print_message(MSGT_WARNING, "skip, \"%s\" exists and it's not a directory.\n", path_to_full);
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
                print_message(MSGT_ERROR, "skip, \"%s\" mkdir failed.\n", path_to_full);
            }  
        }  
    }  
    else  
    {  
        ret_val = OPP_SKIP;  
        print_message(MSGT_WARNING, "skip, \"%s\" is not a file nor directory.\n", path_to_full);
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
  
void CopyThread::install_time()
{

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

    if((num >= 0) && (num <= 3))
    {
        path_to = "/home/wz/test1";
    }
    else if((num >= 4) && (num <= 7))
    {
        path_to = "/home/wz/test2";
    }
    else if((num >= 8) && (num <= 11))
    {
        path_to = "/home/wz/test3";
    }
    else if((num >= 12) && (num <= 15))
    {
        path_to = "/home/wz/test4";
    }

    path_from = dir;  

    walk_sum(path_from, path_to, nullptr);
      
    if(sum.file == 0 && sum.dir == 0)  
    {  
        printf("nothing found.\n");  
    }  
    else  
    {  
        /* �ڶ��α�����ִ��*/  
        copied.file = 0;  
        copied.dir = 0;  
        copied.size = 0;  
  
        // ����һ����ʱ����ÿ��1����ʾһ�½���   
        time(&copy_start_time);  

        path_from = dir;  

        /*Դ�Ƿ����*/  
        if(-1 == stat(path_from, &st_src))  
        {  
            print_message(MSGT_ERROR, "\"%s\" doesn't exist.\n", path_from);  
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
                print_message(MSGT_ERROR, "can't xcp \"%s\" -> \"%s\"\n", path_from, path_to);  
                return 0;  
            }  
        }  
        else  
        {  
            print_message(MSGT_WARNING, "skip \"%s\" not a file nor a directory.\n", path_from);  
            return 0;  
        }  

        walk_copy(path_from, path_to, nullptr);
    }  
  
    return 0;  
}  

