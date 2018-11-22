#include "searchthread.h"

SearchThread::SearchThread(QObject *parent)
{
}

void SearchThread::run()
{
    while(1)
    {
        read_mount_point();
        read_unmount_point();

        sleep(1);
    }
}
