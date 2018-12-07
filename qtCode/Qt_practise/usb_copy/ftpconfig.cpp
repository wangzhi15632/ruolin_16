#include "ftpconfig.h"
#include "ui_ftpconfig.h"

FtpConfig::FtpConfig(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FtpConfig)
{
    ui->setupUi(this);
}

FtpConfig::~FtpConfig()
{
    delete ui;
}
