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
	char mountPoint[20] = {0};
	
	/*查看/media/mount_info 文件，确认新挂载的目录名*/
	fp = fopen("/media/mount_info.txt", "r");
    if(fp == nullptr)
	{
		return;
	}
	else
    {
        qDebug("open mount_info");
        while((read = getline(&line, &len, fp)) != -1)
		{
			memset(mountPoint, 0, 20);
			memcpy(mountPoint, line, read - 1);

            CopyThread *copyThread = new CopyThread(this->window);
            //connect(this, SIGNAL(sendPar(char *)), copyThread, SLOT(rcvPar(char *)));
            copyThread->cp_dir(mountPoint);

            connect(copyThread, SIGNAL(finished()), copyThread, SLOT(deleteLater()));

            copyThread->start();

            //emit(sendPar(mountPoint));
		}
		
		if(line)
			free(line);
		/*读完文件，删除*/
		if(remove("/media/mount_info.txt") == 0)
		{
            qDebug("remove mount_info success\n");
		}
		else
		{
            qDebug("remove mount_info failed\n");
		}
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

    /*查看/media/unmount_info 文件，确认新挂载的目录名*/
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

            if(strlen(unmountPoint) == 11)  // /media/usb0-9
            {
                num = atoi(&unmountPoint[10]);
            }
            else if(strlen(unmountPoint) == 12) // /media/usb10-15
            {
                num = atoi(&unmountPoint[10]);
            }

            qDebug("send num:%d",num);
            emit(sendUnmountNum(num));
        }

        if(line)
            free(line);

        /*读完文件，删除*/
        if(remove("/media/unmount_info.txt") == 0)
        {
            qDebug("remove unmount_info.txt success\n");
        }
        else
        {
            qDebug("remove unmount_info.txt failed\n");
        }
    }
}


