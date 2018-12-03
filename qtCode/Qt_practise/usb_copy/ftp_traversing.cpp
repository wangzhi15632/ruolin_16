#include "ftp_traversing.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <QDebug>
#include <QThread>
#include "mainwindow.h"

char* FtpTraversing::make_path(char *dest, const char *frt, const char *snd)
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

int FtpTraversing::walk_transmisson(const char* path_from, const char* path_tree)
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
        qCritical("can't access \"%s\".\n", path_from_full);
        return OPP_SKIP;
    }

    /*如果是目录，则浏览目录，否则结束*/
    if(!S_ISDIR(st.st_mode))
    {
        return OPP_CONTINUE;
    }

    /*打开目录*/
    if(!(dir = opendir(path_from_full)))
    {
        qCritical("can't open directory \"%s\".\n", path_from_full);
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
            qCritical("skip, can't access %s\"\".\n", path_from_full);
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
            if(walk_transmisson(path_from, path_tree_new) == OPP_CANCEL)
            {
                ret_val = OPP_CANCEL;
                break;
            }
        }
        else
        {
            /*处理函数处理一个子项*/
            if(transmission_action(path_from,path_tree_new, &st) == OPP_CANCEL)
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
int FtpTraversing::transmission_action(const char* path_from, const char* path_tree, const struct stat* st)
{
    int ret_val = OPP_CONTINUE;

    if(S_ISREG(st->st_mode))
    {
        /*与U盘复制线程进行互斥*/
        if(CopyThreadNum.tryAcquire(USB_MAX_NUM) == false)
        {
            ret_val = OPP_CANCEL;
            return ret_val;
        }
        qDebug() << "acquire 16";
        /*与FTP传输线程进行互斥*/
        ftp_mutex.lock();

        make_path(path_from_full, path_from, path_tree);
        qDebug("file:%s", path_from_full);
        emit starFtpPut(path_from_full, nullptr, st->st_size);

        CopyThreadNum.release(USB_MAX_NUM);
    }
    else
    {
        ret_val = OPP_SKIP;
        qWarning("skip, \"%s\" is not a file\n", path_from_full);
    }

    return ret_val;
}


int FtpTraversing::walk_sum(const char* path_from, const char* path_tree)
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
        qWarning("can't access \"%s\".\n", path_from_full);
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
        qWarning("can't open directory \"%s\".\n", path_from_full);
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
            qWarning("skip, can't access %s\"\".\n", path_from_full);
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
            if(walk_sum(path_from, path_tree_new) == OPP_CANCEL)
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
int FtpTraversing::sum_up(const char* path_tree, const struct stat* st)
{
    if(S_ISREG(st->st_mode))
    {
        ftp_sum.file++;
        ftp_sum.size += st->st_size;
    }
    else if(S_ISDIR(st->st_mode))
    {
        ftp_sum.dir++;
    }
    else
    {
        qWarning("skip:%s\n", path_tree);
    }

    return OPP_CONTINUE;
}

FtpTraversing::FtpTraversing()
{

}

void FtpTraversing::transmission_task()
{
    qDebug() << "ftp_thread" << QThread::currentThread();

    char *path_from = nullptr;

    path_from = "/home/wz/test1";

    memset(&ftp_sum, 0, sizeof(ftp_sum));
    //添加for循环
    walk_sum(path_from, nullptr);

    if(ftp_sum.file == 0 && ftp_sum.dir == 0)
    {
        qInfo("nothing found.\n");
    }
    else
    {
        /* 第二次遍历：执行*/
        memset(&ftp_transmission, 0, sizeof(ftp_transmission));
        //记录开始传输时间
        time(&ftp_transmission_start_time);
        //添加for循环
        walk_transmisson(path_from, nullptr);
    }
}
