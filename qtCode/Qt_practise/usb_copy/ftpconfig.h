#ifndef FTPCONFIG_H
#define FTPCONFIG_H

#include <QDialog>
#include <QMessageBox>

namespace Ui {
class FtpConfig;
}

class FtpConfig : public QDialog
{
    Q_OBJECT

public:
    explicit FtpConfig(QWidget *parent = 0);
    ~FtpConfig();

public:
    QString getIPAddr();
    QString getPortAddr();
    QString getUserName();
    QString getPassword();

private:
    Ui::FtpConfig *ui;
};

#endif // FTPCONFIG_H
