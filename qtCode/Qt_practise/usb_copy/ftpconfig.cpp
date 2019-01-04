#include "ftpconfig.h"
#include "ui_ftpconfig.h"
#include <QDebug>

FtpConfig::FtpConfig(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FtpConfig)
{
    ui->setupUi(this);
}

FtpConfig::~FtpConfig()
{
    qDebug() << "ftpConfig";
    delete ui;
}

QString FtpConfig::getIPAddr()
{
    if(!(ui->lineEdit_ip->text().isEmpty()))
        return ui->lineEdit_ip->text();
    else
        QMessageBox::warning(this, tr("确认"), tr("请输入IP地址"));

    return nullptr;
}

QString FtpConfig::getPortAddr()
{
    if(!(ui->lineEdit_port->text().isEmpty()))
        return ui->lineEdit_port->text();
    else
        QMessageBox::warning(this, tr("确认"), tr("请输入端口号"));

    return nullptr;
}

QString FtpConfig::getUserName()
{
    if(!(ui->lineEdit_userName->text().isEmpty()))
        return ui->lineEdit_userName->text();

    return nullptr;
}

QString FtpConfig::getPassword()
{
    if(!(ui->lineEdit_password->text().isEmpty()))
        return ui->lineEdit_password->text();

    return nullptr;
}

