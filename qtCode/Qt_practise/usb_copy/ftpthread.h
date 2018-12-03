#ifndef FTPTHREAD_H
#define FTPTHREAD_H

#include <QUrl>
#include <QFile>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QThread>
#include <QMutex>
#include <QTimer>
#include "usb_copy.h"

class FtpManager: public QObject
{
    Q_OBJECT

public:
    explicit FtpManager();
    // 设置地址和端口
    void setHostPort(const QString &host, int port = 21);
    // 设置登录 FTP 服务器的用户名和密码
    void setUserInfo(const QString &userName, const QString &password);

signals:
    void error(QNetworkReply::NetworkError);
    void sendFtpInfo(QString, sum_t, copied_t, time_t);

private slots:
    // 下载过程中写文件
    void replyFinished(QNetworkReply*);
    void loadError(QNetworkReply::NetworkError);
    // 上传文件
    void put(char *, const QString &, long long);
    void ftpErrorTimeout();
private:
    QFile *file;
    QUrl m_pUrl;
    QByteArray byte_file;
    QNetworkReply *pReply;
    QNetworkAccessManager *accessManager;
    QTimer *ftpTimeOut_timer;
    long long size;
};

#endif // FTPTHREAD_H
