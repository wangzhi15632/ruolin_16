#ifndef FTPWINDOW_H
#define FTPWINDOW_H

#include <QDialog>

namespace Ui {
class FtpWindow;
}

class FtpWindow : public QDialog
{
    Q_OBJECT

public:
    explicit FtpWindow(QWidget *parent = 0);
    ~FtpWindow();

private:
    Ui::FtpWindow *ui;
};

#endif // FTPWINDOW_H
