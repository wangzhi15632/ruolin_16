#include <QFileInfo>
#include "ftpthread.h"
#include "mainwindow.h"
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>

void FtpManager::run()
{
    while(1)
    {
        qDebug("ftp run");
        /*获取到最大资源证明现在没有拷贝线程*/
        CopyThreadNum.acquire(USB_MAX_NUM);
        qDebug("acquire 16");
        emit starCountingDown();
        ftpThreadFlag = 0;
        /*开启倒计时后释放资源*/
        CopyThreadNum.release(USB_MAX_NUM);
        /*等待主线程给信号是FTP上传还是重新获取资源*/
        while(ftpThreadFlag == 0)
        {
            qDebug("wait flag");
            sleep(1);
            continue;
        }

        if(ftpThreadFlag == FTP_REACQURE)
            continue;
        else if(ftpThreadFlag == FTP_UPLOAD)
        {
            /*重新获取信号量，如果获取失败，重头执行*/
            if(CopyThreadNum.tryAcquire(USB_MAX_NUM) == false)
                continue;

            qDebug("star transmission");
            /*开始启动传输任务*/
            transmission_task();
        }
    }
}

char* FtpManager::make_path(char *dest, const char *frt, const char *snd)
{
    if(nullptr == frt || strlen(frt) == 0)
    {
        sprintf(dest, "%s", snd);
    }
    else if(nullptr == snd || strlen(snd) == 0)
    {
        sprintf(dest, "%s", frt);
    }
    else
    {
        if(frt[strlen(frt) - 1] == '/')
        {
            sprintf(dest, "%s%s", frt, snd);
        }
        else
        {
            sprintf(dest, "%s/%s", frt, snd);
        }
    }
    return dest;
}


int FtpManager::transmission_task()
{
    char *path_from = nullptr;

    path_from = "/home/wz/test1";

    //添加for循环
    walk_sum(path_from, nullptr);

    if(sum.file == 0 && sum.dir == 0)
    {
        qInfo("nothing found.\n");
    }
    else
    {
        /* 第二次遍历：执行*/
        memset(&transmission, 0, sizeof(transmission));

        //记录开始传输时间
        time(&transmission_start_time);
        //添加for循环
        walk_transmisson(path_from, nullptr);
    }

    return 0;
}

int FtpManager::walk_transmisson(const char* path_from, const char* path_tree)
{
    struct stat st;
    DIR* dir = nullptr;
    struct dirent *entry = nullptr;
    char path_tree_new[MAX_PATH_LENGTH];
    char path_from_full[MAX_PATH_LENGTH];
    int ret_val = OPP_CONTINUE;

    /*获得源的属性*/
    make_path(path_from_full, path_from, path_tree);
    if(-1 == stat(path_from_full, &st))
    {
        qCritical("can't access \"%s\".\n", path_from_full);
        return OPP_SKIP;
    }

    /*如果是目录，则浏览目录，否则结束*/
    if(!S_ISDIR(st.st_mode))
    {
        return OPP_CONTINUE;
    }

    /*打开目录*/
    if(!(dir = opendir(path_from_full)))
    {
        qCritical("can't open directory \"%s\".\n", path_from_full);
        return OPP_SKIP;
    }

    /*浏览目录*/
    while((entry = readdir(dir)) != nullptr)
    {
        /*构建path_tree_new*/
        make_path(path_tree_new, path_tree, entry->d_name);
        make_path(path_from_full, path_from, path_tree_new);

        /*无法访问 skip*/
        if(-1 == stat(path_from_full, &st))
        {
            qCritical("skip, can't access %s\"\".\n", path_from_full);
            continue;
        }
        /* 忽略 . 和 .. */
        if(S_ISDIR(st.st_mode) && (strcmp(".", entry->d_name) == 0 || strcmp("..", entry->d_name) == 0))
        {
            continue;
        }

        if(S_ISDIR(st.st_mode))
        {
          /*递归处理子目录*/
            if(walk_transmisson(path_from, path_tree_new) == OPP_CANCEL)
            {
                ret_val = OPP_CANCEL;
                break;
            }
        }
        else
        {
            /*处理函数处理一个子项*/
            if(transmission_action(path_from,path_tree_new, &st) == OPP_CANCEL)
            {
                ret_val = OPP_CANCEL;
                break;
            }
        }
    }

    closedir(dir);
    return ret_val;
}

/* 操作 */
int FtpManager::transmission_action(const char* path_from, const char* path_tree, const struct stat* st)
{
    int ret_val = OPP_CONTINUE;
    char path_from_full[MAX_PATH_LENGTH];

    make_path(path_from_full, path_from, path_tree);

    if(S_ISREG(st->st_mode))
    {
        qDebug("file:%s", path_from_full);
        put(path_from_full, "ftp://192.168.1.30/");
    }
    else
    {
        ret_val = OPP_SKIP;
        qWarning("skip, \"%s\" is not a file\n", path_from_full);
    }

    return ret_val;
}


int FtpManager::walk_sum(const char* path_from, const char* path_tree)
{
    struct stat st;
    DIR* dir = nullptr;
    struct dirent *entry = nullptr;
    char path_tree_new[MAX_PATH_LENGTH];
    char path_from_full[MAX_PATH_LENGTH];
    int ret_val = OPP_CONTINUE;

    /*获得源的属性*/
    make_path(path_from_full, path_from, path_tree);
    if(-1 == stat(path_from_full, &st))
    {
        qWarning("can't access \"%s\".\n", path_from_full);
        return OPP_SKIP;
    }

    /*调用一次处理函数，处理当前项*/
    if((ret_val = sum_up(path_tree, &st)) != OPP_CONTINUE)
    {
        return ret_val;
    }

    /*如果是目录，则浏览目录，否则结束*/
    if(!S_ISDIR(st.st_mode))
    {
        return OPP_CONTINUE;
    }

    /*打开目录*/
    if(!(dir = opendir(path_from_full)))
    {
        qWarning("can't open directory \"%s\".\n", path_from_full);
        return OPP_SKIP;
    }

    /*浏览目录*/
    while((entry = readdir(dir)) != nullptr)
    {
        /*构建path_tree_new*/
        make_path(path_tree_new, path_tree, entry->d_name);
        make_path(path_from_full, path_from, path_tree_new);

        /*无法访问 skip*/
        if(-1 == stat(path_from_full, &st))
        {
            qWarning("skip, can't access %s\"\".\n", path_from_full);
            continue;
        }
        /* 忽略 . 和 .. */
        if(S_ISDIR(st.st_mode) && (strcmp(".", entry->d_name) == 0 || strcmp("..", entry->d_name) == 0))
        {
            continue;
        }

        if(S_ISDIR(st.st_mode))
        {
          /*递归处理子目录*/
            if(walk_sum(path_from, path_tree_new) == OPP_CANCEL)
            {
                ret_val = OPP_CANCEL;
                break;
            }
        }
        else
        {
            /*处理函数处理一个子项*/
            if(sum_up(path_tree_new, &st) == OPP_CANCEL)
            {
                ret_val = OPP_CANCEL;
                break;
            }
        }
    }

    closedir(dir);
    return ret_val;
}

/* 统计函数 */
int FtpManager::sum_up(const char* path_tree, const struct stat* st)
{
    if(S_ISREG(st->st_mode))
    {
        sum.file++;
        sum.size += st->st_size;
    }
    else if(S_ISDIR(st->st_mode))
    {
        sum.dir++;
    }
    else
    {
        qWarning("skip:%s\n", path_tree);
    }

    return OPP_CONTINUE;
}

FtpManager::FtpManager(QObject *parent)
{
    // 设置协议
    m_pUrl.setScheme("ftp");
    setHostPort(host, 21);
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
void FtpManager::put(const QString &fileName, const QString &path)
{
    file = new QFile(fileName);
    file->open(QIODevice::ReadOnly);
    QByteArray data = file->readAll();
    m_pUrl.setPath(path);
    pReply = m_manager->put(QNetworkRequest(m_pUrl), data);
    //connect(m_manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
    //connect(pReply, SIGNAL(uploadProgress(qint64, qint64)), this, SIGNAL(uploadProgress(qint64, qint64)));
    connect(pReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SIGNAL(error(QNetworkReply::NetworkError)));
}

// 下载文件
void FtpManager::get(const QString &path, const QString &fileName)
{
    QFileInfo info;
    info.setFile(fileName);
    m_file.setFileName(fileName);
    m_file.open(QIODevice::WriteOnly | QIODevice::Append);
    m_pUrl.setPath(path);
    QNetworkReply *pReply = m_manager->get(QNetworkRequest(m_pUrl));
    //connect(pReply, SIGNAL(finished()), this, SLOT(finished()));
    connect(m_manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
    connect(pReply, SIGNAL(downloadProgress(qint64, qint64)), this, SIGNAL(downloadProgress(qint64, qint64)));
    connect(pReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SIGNAL(error(QNetworkReply::NetworkError)));
}
#if 0
// 下载过程中写文件
void FtpManager::finished()
{
    QNetworkReply *pReply = qobject_cast<QNetworkReply *>(sender());
    switch (pReply->error())
    {
    case QNetworkReply::NoError : {
        m_file.write(pReply->readAll());
        m_file.flush();
    }
        break;

    default:
        break;
    }

    m_file.close();
    pReply->deleteLater();
}
#endif

void FtpManager::replyFinished(QNetworkReply*) //删除指针，更新和关闭文件
{
    if(pReply->error() == QNetworkReply::NoError)
    {
        pReply->deleteLater();
        file->flush();
        file->close();
    }
    else
    {
        QMessageBox::critical(NULL, tr("Error"), "Failed!!!");
    }
}

void FtpManager::ftpStatusFlag(int flag)
{
    ftpThreadFlag = flag;
}
