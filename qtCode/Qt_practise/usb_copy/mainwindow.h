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
#include "ftp_traversing.h"
#include <QNetworkReply>
#include "ftpconfig.h"

#define USB_MAX_NUM 16

extern QSemaphore CopyThreadNum;  /*当FTP线程获取到16个CopyThreadNum信号量，才可以进行传输，表示当前没有复制线程*/
extern bool ftpFlag;     /*ftpFlag 用来判断是否可以发送信号启动FTP线程*/
extern QMutex mutex;  /*互斥信号mutex用来对全局变量ftpFlag变量进行互斥操作*/
extern QMutex ftp_mutex; /*互斥信号ftp_mutex用来对FTP遍历线程和FTP传输线程进行互斥，只有传输完一个文件才能传输下一个文件*/
extern char path_from_full[MAX_PATH_LENGTH];/*标识FTP传输文件目录*/
extern char *path[8];/*复制目录*/
extern sum_t ftp_sum;
extern copied_t ftp_transmission;
extern time_t ftp_transmission_start_time;

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

    QThread *ftpThread;/*ftp传输数据线程*/
    FtpManager *ftpWork;
    QThread *ftpTraverThread;/*ftp遍历目录线程*/
    FtpTraversing *ftpTraver;

    QProgressBar *ftpProgressBar;
    QTimer *timer;/*timer定时器用来显示本地存储多界面，超时更新*/
    QTimer *timer_ftp;/*timer_ftp定时器用来倒计时，时间到了之后群启动FTP遍历线程*/

    FtpConfig *ftpCfg;

private:
    Ui::MainWindow *ui;

signals:
    void starFtpTransmission();

private slots:
    void slotShow(int, unsigned long, unsigned long, unsigned long);/*绘画饼状图*/
    void slotProgress(int, sum_t, copied_t, time_t);/*复制进度*/
    void slotCloseDev(int num);/*拔出一个USB设备*/
    void slotFindDev(char *mountPoint);/*插入一个USB设备*/
    void showLocalStorage();/*显示本地存储界面*/
    void emitToFtpTranslation();/*定时时间到，开启FTP遍历线程*/
    void starFtpTime();/*开启定时器，进行启动FTP遍历线程倒计时*/
    void updateFtpProgress(QString, sum_t, copied_t, time_t); /*更新FTP进度条以及文本框信息*/
    void ftpCfgBtnClicked();
    void test();

};

#endif // MAINWINDOW_H
