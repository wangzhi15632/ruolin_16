#include "copythread.h"
#include <sys/statfs.h>
#include <stdio.h>
#include <stdlib.h>
#include <QMetaType>

CopyThread::CopyThread(MainWindow* window)
{
    wind = window;
    qRegisterMetaType<sum_t>("sum_t");
    qRegisterMetaType<copied_t>("copied_t");
    qRegisterMetaType<time_t>("time_t");

    connect(this, SIGNAL(sendUDevInfo(int, unsigned long, unsigned long, unsigned long)), wind, SLOT(slotShow(int, unsigned long, unsigned long, unsigned long)));
    connect(this, SIGNAL(sendToUI(int, sum_t, copied_t, time_t, bool)), wind, SLOT(slotProgress(int, sum_t, copied_t, time_t, bool)));

}

void CopyThread::rcvPar(char *mountPoint)
{
    qDebug("rcv part");
    memcpy(mountDir, mountPoint, 20);
    rcvFlag = true;
}

void CopyThread::timeOutEmit(void)
{
    //qDebug("xxx");
    //emit(sendToUI(sum, copied, copy_start_time, false));
}

void CopyThread::run()
{
    if(rcvFlag)
    {
        struct statfs s;
        memset(&s, 0, sizeof(struct statfs));

        if(0 != statfs(mountDir, &s))
            return;

        num = atoi(&mountDir[strlen(mountDir) - 1]);
        emit(sendUDevInfo(num, s.f_blocks, s.f_bsize, s.f_bfree));

        cp_task(mountDir);
    }
    rcvFlag = false;
}
