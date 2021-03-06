#include "copythread.h"
#include <sys/statfs.h>
#include <stdio.h>
#include <stdlib.h>
#include <QMetaType>

CopyThread::CopyThread()
{
    rcvFlag = false;
}

void CopyThread::cp_dir(char *mountPoint)
{
    memcpy(mountDir, mountPoint, 20);
    rcvFlag = true;
}

void CopyThread::timeOutEmit(void)
{
    qDebug("aaaaaa");
    //emit(sendToUI(sum, copied, copy_start_time, false));
}

void CopyThread::run()
{
    if(rcvFlag)
    {
        struct statfs s;
        memset(&s, 0, sizeof(struct statfs));

        qDebug("mountDir:%s", mountDir);
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
}
