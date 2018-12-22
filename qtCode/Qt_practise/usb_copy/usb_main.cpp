#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <pthread.h>
#include "usb_copy.h"

#include "config.h"
#include "mainwindow.h"
#include "searchthread.h"
#include "copythread.h"

void SearchThread::read_mount_point(void)
{
    FILE *fp;
    size_t  len = 0;
    ssize_t read;
    char *line = nullptr;

    fp = fopen("/media/mount_info.txt", "r");
    if(fp == nullptr)
    {
        return;
    }
    else
    {
        while((read = getline(&line, &len, fp)) != -1)
        {
            memset(mountPoint, 0, 20);
            memcpy(mountPoint, line, read - 1);

            emit(sendMountNum(mountPoint));
        }

        if(line)
            free(line);

        if(remove("/media/mount_info.txt") != 0)
            qCritical("remove mount_info failed\n");
    }
}

void SearchThread::read_unmount_point(void)
{
    FILE *fp;
    size_t  len = 0;
    ssize_t read;
    int num;
    char *line = nullptr;
    char unmountPoint[20] = {0};

    fp = fopen("/media/unmount_info.txt", "r");
    if(fp == nullptr)
    {
        return;
    }
    else
    {
        while((read = getline(&line, &len, fp)) != -1)
        {
            memset(unmountPoint, 0, 20);
            memcpy(unmountPoint, line, read - 1);

            num = atoi(&unmountPoint[10]);

            emit(sendUnmountNum(num));
        }

        if(line)
            free(line);

        if(remove("/media/unmount_info.txt") != 0)
            qCritical("remove unmount_info.txt failed\n");
    }
}

void SearchThread::check_ftp_transmission(void)
{
    if(ftpFlag == false)
        return;

    if(CopyThreadNum.tryAcquire(USB_MAX_NUM) == true)
    {
        emit starCountingDown();
        CopyThreadNum.release(USB_MAX_NUM);

        mutex.lock();
        ftpFlag = false;
        mutex.unlock();
    }
}



