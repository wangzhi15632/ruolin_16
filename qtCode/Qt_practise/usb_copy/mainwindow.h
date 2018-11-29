#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QtCharts>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include "usb_copy.h"
#include "searchthread.h"
#include "ftpthread.h"

#define USB_MAX_NUM 16

extern QSemaphore CopyThreadNum;
typedef struct
{
    QPieSlice *slice_1, *slice_2;
    QPieSeries *series;
    QChart *chart;
    QChartView *chartview;
    QVBoxLayout *verticalLayout_1;
    QLabel *label1, *label2, *label3, *label4, *label5, *label6;
    QHBoxLayout *horizontalLayout_1, *horizontalLayout_2, *horizontalLayout_10;
    QSpacerItem *horizontalSpacer, *horizontalSpacer_2, *horizontalSpacer_3;
    QProgressBar *progressBar;

    bool clearFlag;

}usb_t;


typedef struct
{
    QPieSlice *slice_1, *slice_2;
    QPieSeries *series;
    QChart *chart;
    QChartView *chartview;
    QVBoxLayout *verticalLayout_local;
    QLabel *label1, *label2, *label3, *label4;
    QSpacerItem *horizontalSpacer_local;
    QHBoxLayout *horizontalLayout_local;

}local_t;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    void init();
    void initTimer();
    void drawPieChartInit();
    QGroupBox* groupBox(int i);
    char* human_size(long long s, char *hs);

private:
    local_t local;
    usb_t usb[USB_MAX_NUM];
    SearchThread *searchThread;
    FtpManager *ftpThread;
    QProgressBar *ftpProgressBar;
    QTimer *timer;
    QTimer *timer_ftp;

private:
    Ui::MainWindow *ui;

signals:
    void setFtpStatusFlag(int);
private slots:
    void slotShow(int, unsigned long, unsigned long, unsigned long);
    void slotProgress(int, sum_t, copied_t, time_t);
    void slotCloseDev(int num);
    void slotFindDev(char *mountPoint);
    void showLocalStorage();
    void emitToFtpTranslation();
    void starFtpTime();
};

#endif // MAINWINDOW_H
