#ifndef FTPCONFIG_H
#define FTPCONFIG_H

#include <QDialog>
#include <QMessageBox>
#include <QLineEdit>
#include <QTreeWidget>
#include <QDialogButtonBox>
#include <QProgressDialog>
#include <QFile>
#include <QNetworkSession>
#include <QNetworkConfigurationManager>
#include "qftp.h"
#include <sys/stat.h>
#include <sys/statfs.h>
#include <sys/types.h>
QT_BEGIN_NAMESPACE
class QDialogButtonBox;
class QFile;
class QFtp;
class QLabel;
class QLineEdit;
class QTreeWidget;
class QTreeWidgetItem;
class QProgressDialog;
class QPushButton;
class QUrlInfo;
class QNetworkSession;
QT_END_NAMESPACE

class FtpConfig : public QDialog
{
    Q_OBJECT

public:
    explicit FtpConfig(QDialog *parent = 0);
    ~FtpConfig();
    QSize sizeHint() const;

    void ftpTraversing(const char *path);
    void addToList(QString file_name, const struct stat &st);
    char* make_path(char *dest, const char *frt, const char *snd);
    int walk_sum(const char* path_from, const char* path_tree);
    int walk_transmisson(const char* path_from, const char* path_tree);
    int transmission_action(const char* path_from, const char* path_tree, const struct stat* st);
    void transmission_task();
//![0]
private slots:
    void connectOrDisconnect();
    void uploadFile();
    void cancelUpload();
    void connectToFtp();

    void ftpCommandFinished(int commandId, bool error);
    void updateDataTransferProgress(qint64 readBytes,
                                    qint64 totalBytes);
    void enableConnectButton();

//![0]
private:
    QLabel *ftpServerLabel;
    QLineEdit *ftpServerLineEdit;
    QLabel *statusLabel;
    QTreeWidget *fileList;
    QPushButton *connectButton;
    QPushButton *uploadButton;
    QPushButton *quitButton;
    QDialogButtonBox *buttonBox;
    QProgressDialog *progressDialog;

//![1]
    QString currentPath;
    QFtp *ftp;
    QFile *file;
    QByteArray date;

    QNetworkSession *networkSession;
    QNetworkConfigurationManager manager;
//![1]
//!
};

#endif // FTPCONFIG_H
