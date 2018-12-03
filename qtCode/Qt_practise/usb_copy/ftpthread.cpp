#include <QFileInfo>
#include <string.h>
#include "ftpthread.h"
#include "mainwindow.h"

FtpManager::FtpManager()
{
    // 设置协议
    m_pUrl.setScheme("ftp");
    setHostPort("192.168.1.30", 21);
}
// 设置地址和端口
void FtpManager::setHostPort(const QString &host, int port)
{
    m_pUrl.setHost(host);
    m_pUrl.setPort(port);
}
// 设置登录 FTP 服务器的用户名和密码
void FtpManager::setUserInfo(const QString &userName, const QString &password)
{
    m_pUrl.setUserName(userName);
    m_pUrl.setPassword(password);
}
// 上传文件
void FtpManager::put(char *fileName, const QString &path, long long file_size)
{
    char *file_name = nullptr;
    QString temp;
    ftpTimeOut_timer = new QTimer(this);
    ftpTimeOut_timer->start(30000);//10S没有重新赋值则认为连接错误
    connect(ftpTimeOut_timer, SIGNAL(timeout()), this, SLOT(ftpErrorTimeout()));

    temp.append("put    ");
    temp.append(fileName);
    temp.append("....");

    emit sendFtpInfo(temp, ftp_sum, ftp_transmission, ftp_transmission_start_time);
    file = new QFile(fileName);
    file->open(QIODevice::ReadOnly);
    byte_file = file->readAll();
    accessManager = new QNetworkAccessManager(this);
    accessManager->setNetworkAccessible(QNetworkAccessManager::Accessible);

    file_name = strrchr(fileName, '/');
    m_pUrl.setPath(file_name);
    QNetworkRequest request(m_pUrl);

    pReply = accessManager->put(request, byte_file);

    connect(accessManager,SIGNAL(finished(QNetworkReply*)),this,SLOT(replyFinished(QNetworkReply*)));
    connect(pReply, SIGNAL(error(QNetworkReply::NetworkError)),this,SLOT(loadError(QNetworkReply::NetworkError)));

    size = file_size;
}

void FtpManager::loadError(QNetworkReply::NetworkError) //传输中的错误输出
{
    qDebug() <<"Error: " << pReply->error();
    qDebug() << "load error: " << QThread::currentThread();
}

void FtpManager::replyFinished(QNetworkReply*) //删除指针，更新和关闭文件
{
    qDebug() << "FtpManager" << QThread::currentThread();
    QString temp;

    if(pReply->error() == QNetworkReply::NoError)
    {
        file->remove(path_from_full);
        ftp_transmission.size += size;
        temp.append("finished");
    }
    else
    {
        QMessageBox::critical(NULL, tr("Error"), "Failed!!!");
        temp.append("failed");
    }

    emit sendFtpInfo(temp, ftp_sum, ftp_transmission, ftp_transmission_start_time);

    ftpTimeOut_timer->deleteLater();
    pReply->deleteLater();
    file->flush();
    file->close();
    file->deleteLater();
    accessManager->deleteLater();
    qDebug() << "finished";
    ftp_mutex.unlock();
}

void FtpManager::ftpErrorTimeout()
{
    QString temp;

    temp.append("timeout");
    emit sendFtpInfo(temp, ftp_sum, ftp_transmission, ftp_transmission_start_time);
    ftpTimeOut_timer->deleteLater();
    pReply->deleteLater();
    file->flush();
    file->close();
    file->deleteLater();
    accessManager->deleteLater();
    qDebug() << "ftp_timeout";
    ftp_mutex.unlock();
}
