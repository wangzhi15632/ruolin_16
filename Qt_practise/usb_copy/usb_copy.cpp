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
* 复制文件可以显示进度 
*  
* 两个思路:遍历文件一次，把文件名记录在一个列表，后续操作直接从列表中得到文件名 
* 或者遍历两遍，第一次统计，第二次执行 
*  
* 关于进度条  
* 1. 用定时器每隔1秒刷新一次要注意函数重入的问题 
* 2. 两个线程工作线程统计/拷贝主线程刷新状态，似乎小题大做了 
* 3. 一个线程有变化时刷新，这样就无法现实动画 
* 
* TODO 
* 2013-10-22 
* 1. 添加命令行选项的处理。 
* 2. 添加文件无法访问/目录无法创建或者文件/目录已经存在的情况的处理。 
* 3. 如果没有任何文件成功复制时的提示信息BUG（在有文件detected的情况下）。 
* 4. 复制文件，目标是已经存在的目录名时自动添加文件名而不是直接复制。 
* 5. 结束时用human_time 来显示用去的时间。 
*  
* 2013-10-23 
* 1. 统计阶段也要显示动画 
*  
* 2013-10-24 
* 1. overwrite 提示后等待用户输入和定时器冲突的问题 
*  
* 2013-10-29 
* 1. 多源拷贝在主函数做个循环，都要补齐文件名，判断是否存在等. 
* 
*/  

/*启用大文件支持*/  
//#define _LARGEFILE64_SOURCE  
//#define _FILE_OFFSET_BITS 64  
  
//#ifdef _LARGEFILE64_SOURCE  
//#define stat stat64  
//#define fopen fopen64  
//#define fread fread64  
//#define fwrite fwrite64  
//#endif  

/* 全局变量 */  
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
* 先清除状态文字然后在输出信息 
* 1. 状态文字总是在当前行输出不换行 
* 2. printerror只能在状态文字被显示之后输出，即定时器被安装之后使用。 
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
  
/*连接目录字符串,主要处理末尾/的问题,frt snd 两个参数不能同时为空那样没有意义*/  
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
* 遍历函数 
* 遍历函数只保证源文件/文件夹的每一项都调用一次opp函数 
* 由opp函数的返回值决定是否继续扫描 
* 采用“串烧”式程序风格 
* 只有一种情况下返回值为FALSE：opp 函数返回OPP_CANCEL 
*/  
int CopyThread::walk_sum(const char* path_from, const char* path_to, const char* path_tree)
{  
    struct stat st;  
    DIR* dir = nullptr;
    struct dirent *entry = nullptr;
    char path_tree_new[MAX_PATH_LENGTH];  
    char path_from_full[MAX_PATH_LENGTH];  
    int ret_val = OPP_CONTINUE;  
  
    /*获得源的属性*/  
    make_path(path_from_full, path_from, path_tree);  
    if(-1 == stat(path_from_full, &st))  
    {  
        print_message(MSGT_ERROR, "can't access \"%s\".\n", path_from_full);  
        return OPP_SKIP;  
    }  
  
    /*调用一次处理函数，处理当前项*/  
    if((ret_val = sum_up(path_tree, &st)) != OPP_CONTINUE)
    {  
        return ret_val;  
    }  
              
    /*如果是目录，则浏览目录，否则结束*/  
    if(!S_ISDIR(st.st_mode))  
    {  
        return OPP_CONTINUE;  
    }  
  
    /*打开目录*/  
    if(!(dir = opendir(path_from_full)))  
    {  
        print_message(MSGT_ERROR, "can't open directory \"%s\".\n", path_from_full);  
        return OPP_SKIP;  
    }  
  
    /*浏览目录*/  
    while((entry = readdir(dir)) != nullptr)
    {  
        /*构建path_tree_new*/
        make_path(path_tree_new, path_tree, entry->d_name);  
        make_path(path_from_full, path_from, path_tree_new);  
      
        /*无法访问 skip*/  
        if(-1 == stat(path_from_full, &st))  
        {  
            qDebug("skip, can't access %s\"\".\n", path_from_full);
            continue;  
        }
        /* 忽略 . 和 .. */
        if(S_ISDIR(st.st_mode) && (strcmp(".", entry->d_name) == 0 || strcmp("..", entry->d_name) == 0))  
        {  
            continue;  
        }  

        if(S_ISDIR(st.st_mode))  
        {  
          /*递归处理子目录*/  
            if(walk_sum(path_from, path_to, path_tree_new) == OPP_CANCEL)
            {  
                ret_val = OPP_CANCEL;  
                break;  
            }  
        }  
        else  
        {  
            /*处理函数处理一个子项*/  
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
  
/* 统计函数 */  
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

    /*获得源的属性*/
    make_path(path_from_full, path_from, path_tree);
    if(-1 == stat(path_from_full, &st))
    {
        print_message(MSGT_ERROR, "can't access \"%s\".\n", path_from_full);
        return OPP_SKIP;
    }

    /*调用一次处理函数，处理当前项*/
    if((ret_val = action(path_from, path_to, path_tree, &st)) != OPP_CONTINUE)
    {
        return ret_val;
    }

    /*如果是目录，则浏览目录，否则结束*/
    if(!S_ISDIR(st.st_mode))
    {
        return OPP_CONTINUE;
    }

    /*打开目录*/
    if(!(dir = opendir(path_from_full)))
    {
        print_message(MSGT_ERROR, "can't open directory \"%s\".\n", path_from_full);
        return OPP_SKIP;
    }

    /*浏览目录*/
    while((entry = readdir(dir)) != nullptr)
    {
        /*构建path_tree_new*/
        make_path(path_tree_new, path_tree, entry->d_name);
        make_path(path_from_full, path_from, path_tree_new);

        /*无法访问 skip*/
        if(-1 == stat(path_from_full, &st))
        {
            //qDebug("skip, can't access %s\"\".\n", path_from_full);
            continue;
        }
        /* 忽略 . 和 .. */
        if(S_ISDIR(st.st_mode) && (strcmp(".", entry->d_name) == 0 || strcmp("..", entry->d_name) == 0))
        {
            continue;
        }

        if(S_ISDIR(st.st_mode))
        {
          /*递归处理子目录*/
            if(walk_copy(path_from, path_to, path_tree_new) == OPP_CANCEL)
            {
                ret_val = OPP_CANCEL;
                break;
            }
        }
        else
        {
            /*处理函数处理一个子项*/
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
/* 操作 */  
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
                            /*只有部分文件被复制也视为成功因为文件系统中已经有这个文件的记录了*/  
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
  
/*禁止循环复制，即目标文件/文件夹不能包含在源文件/文件夹中*/  
bool CopyThread::is_self_copy(const char* src, const char* dest)
{  
    /*严格的做法应该先把src和dest都转化为绝对路径然后在比较，不过 
     *Linux下的相对路径比较麻烦有 ~ ./ ../ ../../ 等... 
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

/*主函数，做两次遍历*/  
int CopyThread::cp_task(char *dir)
{  
    struct stat st_src, st_dest;  
    char path_to_fixed[MAX_PATH_LENGTH];  
    char *path_from = nullptr, *path_to = nullptr, *file_name = nullptr;
 
    /* 第一次遍历：统计 */  
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
        /* 第二次遍历：执行*/  
        copied.file = 0;  
        copied.dir = 0;  
        copied.size = 0;  
  
        // 设置一个定时器，每隔1秒显示一下进度   
        time(&copy_start_time);  

        path_from = dir;  

        /*源是否存在*/  
        if(-1 == stat(path_from, &st_src))  
        {  
            print_message(MSGT_ERROR, "\"%s\" doesn't exist.\n", path_from);  
            return 0;  
        }  
      
        /* 
        * 如果源是文件而且目标是已经存在的目录，则自动补齐文件名 
        * 如果目标是已经存在的文件，先判断是否指向同一个文件 inode number 
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
                /*源是目录时要防止循环复制*/  
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

