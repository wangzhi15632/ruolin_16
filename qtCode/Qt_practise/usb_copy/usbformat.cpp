#include "usbformat.h"
#include "ui_usbformat.h"
#include "mainwindow.h"

usbFormat::usbFormat(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::usbFormat)
{
    ui->setupUi(this);
    init();
}

usbFormat::~usbFormat()
{
    delete ui;
}

void usbFormat::init()
{
    setWindowTitle(tr("SD卡格式化配置"));
    connect(ui->buttonGroup, SIGNAL(buttonClicked(QAbstractButton *)), this, SLOT(slotButtonGroupClicked(QAbstractButton *)));
}

void usbFormat::slotButtonGroupClicked(QAbstractButton *pBtn)
{
    if(pBtn == ui->checkBox_yes)
    {
        is_format_usb = true;
    }
    else if(pBtn == ui->checkBox_no)
    {
        is_format_usb = false;
    }
}
