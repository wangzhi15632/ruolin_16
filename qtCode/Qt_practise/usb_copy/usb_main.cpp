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
	
	/*�鿴/media/mount_info �ļ���ȷ���¹��ص�Ŀ¼��*/
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

            CopyThread *copyThread = new CopyThread(this->window);
            connect(this, SIGNAL(sendPar(char *)), copyThread, SLOT(rcvPar(char *)));
            connect(copyThread, SIGNAL(finished()), copyThread, SLOT(deleteLater()));
            copyThread->start();

            //emit(sendPar(mountPoint));
        }
		
		if(line)
			free(line);

		/*�����ļ���ɾ��*/
		if(remove("/media/mount_info.txt") == 0)
		{
			printf("remove success\n");
		}
		else
		{
			printf("remove failed\n");
		}
	}
}

