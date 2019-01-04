#include "searchthread.h"
#include <QDebug>

SearchThread::SearchThread(QObject *parent)
{
    qDebug() << "searchThread";
}

SearchThread::~SearchThread()
{
    requestInterruption();
    quit();
    wait();

    qDebug() << "delete searchThread";
}

void SearchThread::run()
{
    while(!isInterruptionRequested())
    {
        read_mount_point();

        read_unmount_point();

        //check_ftp_transmission();
    }

    qDebug() << "search quit";
}
