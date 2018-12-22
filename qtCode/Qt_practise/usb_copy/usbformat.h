#ifndef USBFORMAT_H
#define USBFORMAT_H

#include <QDialog>
#include <QAbstractButton>
#include <QButtonGroup>

namespace Ui {
class usbFormat;
}

class usbFormat : public QDialog
{
    Q_OBJECT

public:
    explicit usbFormat(QWidget *parent = 0);
    ~usbFormat();
    void init();

private slots:
    void slotButtonGroupClicked(QAbstractButton *pBtn);

private:
    Ui::usbFormat *ui;
};

#endif // USBFORMAT_H
