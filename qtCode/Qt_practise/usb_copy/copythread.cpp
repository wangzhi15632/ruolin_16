#include "copythread.h"
#include <sys/statfs.h>
#include <stdio.h>
#include <stdlib.h>
#include <QMetaType>

CopyThread::CopyThread()
{
    rcvFlag = false;
    transcodingFlag = true; //默认进行转码
}

void CopyThread::test()
{
    qDebug("end!!!! num:%d", this->num);
    qDebug() << "copy thread" << currentThread();
    qDebug() << "this:"<< this;
}

void CopyThread::cp_dir(char *mountPoint)
{
    memcpy(mountDir, mountPoint, 20);
    rcvFlag = true;
}

void CopyThread::run()
{
    CopyThreadNum.acquire();

    if(rcvFlag)
    {
        struct statfs s;
        memset(&s, 0, sizeof(struct statfs));

        if(0 != statfs(mountDir, &s))
            return;

        if(strlen(mountDir) == 11)
        {
            num = atoi(&mountDir[10]);
        }
        else if(strlen(mountDir) == 12)
        {
            num = atoi(&mountDir[10]);
        }

        emit(sendUDevInfo(num, s.f_blocks, s.f_bsize, s.f_bfree));

        cp_task(mountDir);
    }
    rcvFlag = false;

    CopyThreadNum.release();

    mutex.lock();
    ftpFlag = true;
    mutex.unlock();
}
