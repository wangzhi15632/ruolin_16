#ifndef FTPTHREAD_H
#define FTPTHREAD_H

#include <QUrl>
#include <QFile>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QThread>

#define FTP_REACQURE 1
#define FTP_UPLOAD  2

typedef struct ftpSum
{
    int file;
    int dir;
    long long size;
}ftpSum_t;

typedef struct ftpTransmission
{
    int file;
    int dir;
    long long size;
}ftpTransmission_t;

class FtpManager : public QThread
{

    Q_OBJECT
public:
    explicit FtpManager(QObject *parent);
    // 设置地址和端口
    void setHostPort(const QString &host, int port = 21);
    // 设置登录 FTP 服务器的用户名和密码
    void setUserInfo(const QString &userName, const QString &password);
    // 上传文件
    void put(const QString &fileName, const QString &path);
    // 下载文件
    void get(const QString &path, const QString &fileName);

    //启动传输任务
    int transmission_task();
    //遍历汇总
    int walk_sum(const char* path_from, const char* path_tree);
    int sum_up(const char* path_tree, const struct stat* st);
    //遍历传输
    int walk_transmisson(const char* path_from, const char* path_tree);
    int transmission_action(const char* path_from, const char* path_tree, const struct stat* st);

    char* make_path(char *dest, const char *frt, const char *snd);


    void run();

signals:
    void error(QNetworkReply::NetworkError);
    // 上传进度
    void uploadProgress(qint64 bytesSent, qint64 bytesTotal);
    // 下载进度
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    //开始倒计时
    void starCountingDown();

private slots:
    // 下载过程中写文件
    void replyFinished(QNetworkReply*);
    void ftpStatusFlag(int);

private:
    QUrl m_pUrl;
    QFile m_file;
    QNetworkAccessManager *m_manager;
    QNetworkReply *pReply;

    uint8_t ftpThreadFlag = 0;  //1：倒计时被打断，FTP线程需重新获取资源.2：倒计时正常结束，FTP线程开始上传数据
    ftpSum_t sum;
    ftpTransmission_t transmission;
    time_t transmission_start_time;

    QString ftpDir;
    QString host;

    QFile *file;
    QByteArray byte_file;
};

#endif // FTPTHREAD_H
