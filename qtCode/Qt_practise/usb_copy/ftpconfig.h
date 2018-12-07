#ifndef FTPCONFIG_H
#define FTPCONFIG_H

#include <QDialog>

namespace Ui {
class FtpConfig;
}

class FtpConfig : public QDialog
{
    Q_OBJECT

public:
    explicit FtpConfig(QWidget *parent = 0);
    ~FtpConfig();

private:
    Ui::FtpConfig *ui;
};

#endif // FTPCONFIG_H
