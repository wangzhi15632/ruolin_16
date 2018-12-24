#include "ftpwindow.h"
#include "ui_ftpwindow.h"

FtpWindow::FtpWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FtpWindow)
{
    ui->setupUi(this);
}

FtpWindow::~FtpWindow()
{
    delete ui;
}
