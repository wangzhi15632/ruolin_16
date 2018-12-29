#ifndef FTPCONFIG_H
#define FTPCONFIG_H

#include <QDialog>
#include <QMessageBox>
#include <QLineEdit>
#include <QTreeWidget>
#include <QDialogButtonBox>
#include <QProgressDialog>
#include <QFile>
#include <sys/stat.h>
#include <sys/statfs.h>
#include <sys/types.h>
#include <QNetworkSession>
#include <QNetworkConfigurationManager>

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
    explicit FtpConfig(QWidget *parent = 0);
    ~FtpConfig();
    QSize sizeHint() const;

private:
    void ftpTraversing(const char *path);
    void addToList(QString file_name, const struct stat &st);
    char* make_path(char *dest, const char *frt, const char *snd);
    int walk_sum(const char* path_from, const char* path_tree);

private slots:
    void connectOrDisconnect();
    void uploadFile();
    void cancelUpload();
    void connectToFtp();

    void ftpCommandFinished(int commandId, bool error);
    void updateDataTransferProgress(qint64 readBytes,
                                    qint64 totalBytes);
    void enableConnectButton();

private:
    QLineEdit *ftpServerLineEdit;
    QTreeWidget *fileList;
    QLabel *ftpServerLabel;
    QLabel *statusLabel;
    QPushButton *connectButton;
    QPushButton *uploadButton;
    QPushButton *quitButton;
    QDialogButtonBox *buttonBox;
    QProgressDialog *progressDialog;

public:
    QFtp *ftp;
    QFile *file;
    QString server_edit;
    QByteArray date;
    QNetworkSession *networkSession;
    QNetworkConfigurationManager manager;
    QThread *ftp_thread;

};

#endif // FTPCONFIG_H
