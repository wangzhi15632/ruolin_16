#include "ftpconfig.h"
#include "qftp.h"
#include "ftp_traversing.h"
#include "mainwindow.h"
#include <sys/stat.h>
#include <QtWidgets>
#include <QtNetwork>
#include <QDebug>
#include <sys/types.h>
#include <dirent.h>

FtpConfig::FtpConfig(QWidget *parent)
    : QDialog(parent), ftp(0), networkSession(0)
{
    ftpServerLabel = new QLabel(tr("Ftp &server:"));
    ftpServerLineEdit = new QLineEdit("192.168.1.30");
    ftpServerLabel->setBuddy(ftpServerLineEdit);

    statusLabel = new QLabel(tr("Please enter the name of an FTP server."));

    fileList = new QTreeWidget;
    fileList->setEnabled(false);
    fileList->setRootIsDecorated(false);
    fileList->setHeaderLabels(QStringList() << tr("Name") << tr("Size") << tr("Progress") << tr("flag"));
    fileList->header()->setStretchLastSection(true);
    fileList->setColumnWidth(0,800);
    fileList->setColumnWidth(2,250);

    connectButton = new QPushButton(tr("Connect"));
    connectButton->setDefault(true);

    uploadButton = new QPushButton(tr("Upload"));
    uploadButton->setEnabled(false);

    quitButton = new QPushButton(tr("Quit"));

    buttonBox = new QDialogButtonBox;
    buttonBox->addButton(uploadButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(quitButton, QDialogButtonBox::RejectRole);

   // progressDialog = new QProgressDialog(this);
#if 0
    connect(progressDialog, SIGNAL(canceled()), this, SLOT(cancelUpload()));
 #endif
    connect(connectButton, SIGNAL(clicked()), this, SLOT(connectOrDisconnect()));
    connect(uploadButton, SIGNAL(clicked()), this, SLOT(uploadFile()));
    connect(quitButton, SIGNAL(clicked()), this, SLOT(close()));

    QHBoxLayout *topLayout = new QHBoxLayout;
    topLayout->addWidget(ftpServerLabel);
    topLayout->addWidget(ftpServerLineEdit);
    topLayout->addWidget(connectButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(topLayout);
    mainLayout->addWidget(fileList);
    mainLayout->addWidget(statusLabel);
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);

    setWindowTitle(tr("FTP"));

}

FtpConfig::~FtpConfig()
{

}

QSize FtpConfig::sizeHint() const
{
    return QSize(1500, 800);
}


/*遍历要上传的目录*/
void FtpConfig::ftpTraversing(const char* path)
{
    fileList->clear();
    walk_sum(path, nullptr);
}

void FtpConfig::connectOrDisconnect()
{
    if (ftp) {
        ftp->abort();
        ftp->deleteLater();
        ftp = 0;

        fileList->setEnabled(false);
        uploadButton->setEnabled(false);
        uploadButton->setText(tr("Upload"));
        //cancelUpload();
        connectButton->setEnabled(true);
        connectButton->setText(tr("Connect"));

#ifndef QT_NO_CURSOR
        setCursor(Qt::ArrowCursor);
#endif
        statusLabel->setText(tr("Please enter the name of an FTP server."));
        return;
    }

    ftpTraversing("/usb_copy_dir");

#ifndef QT_NO_CURSOR
    setCursor(Qt::WaitCursor);
#endif

    if (!networkSession || !networkSession->isOpen()) {
        if (manager.capabilities() & QNetworkConfigurationManager::NetworkSessionRequired) {
            if (!networkSession) {
                // Get saved network configuration
                QSettings settings(QSettings::UserScope, QLatin1String("Trolltech"));
                settings.beginGroup(QLatin1String("QtNetwork"));
                const QString id = settings.value(QLatin1String("DefaultNetworkConfiguration")).toString();
                settings.endGroup();

                // If the saved network configuration is not currently discovered use the system default
                QNetworkConfiguration config = manager.configurationFromIdentifier(id);
                if ((config.state() & QNetworkConfiguration::Discovered) !=
                    QNetworkConfiguration::Discovered) {
                    config = manager.defaultConfiguration();
                }

                networkSession = new QNetworkSession(config, this);
                connect(networkSession, SIGNAL(opened()), this, SLOT(connectToFtp()));
                connect(networkSession, SIGNAL(error(QNetworkSession::SessionError)), this, SLOT(enableConnectButton()));
            }

            connectButton->setEnabled(false);
            statusLabel->setText(tr("Opening network session."));
            networkSession->open();
            return;
        }
    }
    connectToFtp();
}


void FtpConfig::connectToFtp()
{
    ftp = new QFtp(this);

    //ftp->moveToThread(ftp_thread);
    connect(ftp, SIGNAL(commandFinished(int,bool)),
            this, SLOT(ftpCommandFinished(int,bool)));

    connect(ftp, SIGNAL(dataTransferProgress(qint64,qint64)),
            this, SLOT(updateDataTransferProgress(qint64,qint64)));

    QUrl url(ftpServerLineEdit->text());
    if (!url.isValid() || url.scheme().toLower() != QLatin1String("ftp")) {
        ftp->connectToHost(ftpServerLineEdit->text(), 21);
        ftp->login();
    } else {
        ftp->connectToHost(url.host(), url.port(21));

        if (!url.userName().isEmpty())
            ftp->login(QUrl::fromPercentEncoding(url.userName().toLatin1()), url.password());
        else
            ftp->login();
        if (!url.path().isEmpty())
            ftp->cd(url.path());
    }

    fileList->setEnabled(true);

    connectButton->setEnabled(false);
    connectButton->setText(tr("Disconnect"));
    statusLabel->setText(tr("Connecting to FTP server %1...")
                         .arg(ftpServerLineEdit->text()));
}


void FtpConfig::uploadFile()
{
    QString fileName;
    bool bRet = false;
    int nVal;

    uploadButton->setEnabled(false);

    QTreeWidgetItemIterator it(fileList);
    while(*it){

        bRet = false;
        nVal = 100;
        while(bRet == false)
        {
            nVal--;
            if(nVal == 0)
                break;

            qApp->processEvents();

            bRet = ftp_Sem.tryAcquire(1,100);
            qDebug() << "ftp_quire" << bRet;
        }

        fileName = (*it)->text(0);
        ++it;
        qDebug() << "fileName" << fileName;
        file = new QFile(fileName);
        file->open(QIODevice::ReadOnly);
        date = fileName.toLatin1();
        ftp->put(file, strrchr(date.data(), '/'));
       //ftp->put(file, "e.txt");
    }
    //progressDialog->setLabelText(tr("Uploading %1...").arg(fileName));
   // progressDialog->exec();
}

void FtpConfig::cancelUpload()
{
    ftp->abort();

    if(file != nullptr)
    {
        if (file->exists()) {
            file->close();
        }
        delete file;
    }
}

void FtpConfig::ftpCommandFinished(int, bool error)
{
    qDebug() << "finished";
#ifndef QT_NO_CURSOR
    setCursor(Qt::ArrowCursor);
#endif

    if (ftp->currentCommand() == QFtp::ConnectToHost) {
        if (error) {
            QMessageBox::information(this, tr("FTP"),
                                     tr("Unable to connect to the FTP server "
                                        "at %1. Please check that the host "
                                        "name is correct.")
                                     .arg(ftpServerLineEdit->text()));
            connectOrDisconnect();
            return;
        }
        statusLabel->setText(tr("Logged onto %1.")
                             .arg(ftpServerLineEdit->text()));
        fileList->setFocus();
        uploadButton->setEnabled(true);
        connectButton->setEnabled(true);
        return;
    }

    if (ftp->currentCommand() == QFtp::Login)
    {
        ;
    }

    if (ftp->currentCommand() == QFtp::Put) {
        qDebug() << "delete";
        if (error) {
            statusLabel->setText(tr("Canceled upload of %1.")
                                 .arg(file->fileName()));
            file->close();
        } else {
            statusLabel->setText(tr("Uploaded %1 to current directory.")
                                 .arg(file->fileName()));

            file->close();
            file->remove();
        }
        qDebug() << "ftp_release";

        ftp_Sem.release();

        delete file;
    }
}

void FtpConfig::addToList(QString file_name, const struct stat &st)
{
    QTreeWidgetItem *item = new QTreeWidgetItem;
    item->setText(0, file_name);
    item->setText(1, QString::number(st.st_size));
    item->setText(3, "false");

    QPixmap pixmap(":/images/file.png");
    item->setIcon(0, pixmap);

    fileList->addTopLevelItem(item);
    if (!fileList->currentItem()) {
        fileList->setCurrentItem(fileList->topLevelItem(0));
        fileList->setEnabled(true);
    }
}

void FtpConfig::updateDataTransferProgress(qint64 readBytes,
                                           qint64 totalBytes)
{
   // progressDialog->setMaximum(totalBytes);
  //  progressDialog->setValue(readBytes);
}

void FtpConfig::enableConnectButton()
{
    // Save the used configuration
    QNetworkConfiguration config = networkSession->configuration();
    QString id;
    if (config.type() == QNetworkConfiguration::UserChoice)
        id = networkSession->sessionProperty(QLatin1String("UserChoiceConfiguration")).toString();
    else
        id = config.identifier();

    QSettings settings(QSettings::UserScope, QLatin1String("Trolltech"));
    settings.beginGroup(QLatin1String("QtNetwork"));
    settings.setValue(QLatin1String("DefaultNetworkConfiguration"), id);
    settings.endGroup();

    connectButton->setEnabled(true);
    statusLabel->setText(tr("Please enter the name of an FTP server."));
}


char* FtpConfig::make_path(char *dest, const char *frt, const char *snd)
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


int FtpConfig::walk_sum(const char* path_from, const char* path_tree)
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

    /*如果是目录，则浏览目录，否则结束*/
    if(!S_ISDIR(st.st_mode))
    {
        addToList(path_from_full, st);
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
            addToList(path_from_full, st);
        }
    }

    closedir(dir);
    return ret_val;
}

