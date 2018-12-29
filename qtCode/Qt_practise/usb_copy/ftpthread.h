#ifndef FTPTHREAD_H
#define FTPTHREAD_H

#include <QUrl>
#include <QFile>
#include <QTreeWidget>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QThread>
#include <QMutex>
#include <QTimer>
#include "usb_copy.h"

#include "qftp.h"

class FtpManager: public QObject
{
    Q_OBJECT

public:
    explicit FtpManager();


};

#endif // FTPTHREAD_H
