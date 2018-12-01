#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "usb_main.h"
#include "searchthread.h"
#include "copythread.h"
#include "ftpthread.h"
#include <QChartView>
#include <sys/statfs.h>
#include <QSemaphore>
#include <QThread>

QSemaphore CopyThreadNum(USB_MAX_NUM);
bool ftpFlag = true;
QMutex mutex;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    init();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slotFindDev(char *mountPoint)
{
    qInfo("find a usb dev moutpoint:%s", mountPoint);

    if(timer_ftp->isActive())
    {
        timer_ftp->stop();
    }

    CopyThread *copyThread = new CopyThread();//can't add this parameter

    connect(copyThread, SIGNAL(sendUDevInfo(int, unsigned long, unsigned long, unsigned long)), this, SLOT(slotShow(int, unsigned long, unsigned long, unsigned long)));
    connect(copyThread, SIGNAL(sendToUI(int, sum_t, copied_t, time_t, bool)), this, SLOT(slotProgress(int, sum_t, copied_t, time_t)));
    connect(copyThread, SIGNAL(finished()), copyThread, SLOT(deleteLater()));
    connect(copyThread, SIGNAL(finished()), copyThread, SLOT(test()));

    copyThread->cp_dir(mountPoint);

    copyThread->start();
}

void MainWindow::slotCloseDev(int num)
{
    usb[num].clearFlag = true;

    usb[num].slice_1->setValue(1);
    usb[num].slice_1->setBrush(Qt::darkGray);
    usb[num].slice_2->setValue(0);

    usb[num].label2->clear();
    usb[num].label4->clear();
    usb[num].label6->clear();
    usb[num].progressBar->setValue(0);

    qInfo("close a usb dev %d", num);
}

void MainWindow::slotShow(int i, unsigned long block,unsigned long bsize,unsigned long bavail)
{
    char disk_avail[20];
    char disk_size[20];

    usb[i].clearFlag = false;

    human_size(block * bsize, disk_size);
    human_size(block * bsize - bavail * bsize, disk_avail);

    usb[i].label2->setText(disk_size);
    usb[i].label4->setText(disk_avail);

    usb[i].slice_1->setValue(static_cast<double>(bavail)/(static_cast<double>(block)));
    usb[i].slice_1->setBrush(Qt::lightGray);
    usb[i].slice_2->setValue((1-  static_cast<double>(bavail)/static_cast<double>(block)));
    usb[i].slice_2->setBrush(Qt::blue);

    qInfo("draw usb %d pie", i);

}
/*显示为可读数字*/
void MainWindow::showLocalStorage()
{
    struct statfs s;
    char disk_avail[20] = {0};
    char disk_size[20] = {0};

    memset(&s, 0, sizeof(struct statfs));

    if(0 != statfs("/home/wz/test1", &s))
        return;

    human_size(s.f_blocks * s.f_bsize, disk_size);
    human_size(s.f_blocks * s.f_bsize - s.f_bfree * s.f_bsize, disk_avail);

    local.label2->setText(disk_size);
    local.label4->setText(disk_avail);

    local.slice_1->setValue(static_cast<double>(s.f_bfree)/(static_cast<double>(s.f_blocks)));
    local.slice_1->setBrush(Qt::lightGray);
    local.slice_2->setValue((1-  static_cast<double>(s.f_bfree)/static_cast<double>(s.f_blocks)));
    local.slice_2->setBrush(Qt::blue);
}

void MainWindow::emitToFtpTranslation()
{
    timer_ftp->stop();

    emit starFtpTransmission();
}

void MainWindow::starFtpTime()
{
    //FTP上传倒计时300S
    timer_ftp->start(3000);
}
char* MainWindow::human_size(long long s, char *hs)
{
    if(s >= GBYTES)
    {
        sprintf(hs, "%.2fGB", (s * 1.0) / GBYTES);
    }
    else if(s >= MBYTES)
    {
        sprintf(hs, "%.2fMB", (s * 1.0) / MBYTES);
    }
    else if(s > KBYTES)
    {
        sprintf(hs, "%.2fKB", (s * 1.0) / KBYTES);
    }
    else
    {
        sprintf(hs, "%lldB", s);
    }

    return hs;
}

void MainWindow::slotProgress(int i, sum_t sum, copied_t copied, time_t copy_start_time)
{
    time_t cur_time;
    int percent;
    char hs[20];
    long long sp = 0;
    char speed[20];

    if(usb[i].clearFlag == true)
        return;

    time(&cur_time);
    if(sum.size == 0)
    {
        percent = 0;
    }
    else
    {
        percent = static_cast<int>((copied.size * 1.0 / sum.size) * 100);
    }

    if(cur_time > copy_start_time)
    {
        sp = copied.size / (cur_time - copy_start_time);
        sprintf(speed, "%s/s", human_size(sp, hs));
    }
    else
    {
        sprintf(speed, "-");
    }

    usb[i].label6->setText(speed);
    usb[i].progressBar->setValue(percent);

}

void MainWindow::init()
{
    qDebug() << "main thread: " << QThread::currentThread();

    /*config mainWindow size and title*/
    setWindowTitle(tr("16路转储平台 V1.0"));
    setMinimumSize(QSize(1000, 800));

    /*regist signal-slot parameter*/
    qRegisterMetaType<sum_t>("sum_t");
    qRegisterMetaType<copied_t>("copied_t");
    qRegisterMetaType<time_t>("time_t");

    /*config centralWidget background*/
    QPalette pal(ui->centralWidget->palette());
    pal.setColor(QPalette::Background, Qt::white);
    ui->centralWidget->setAutoFillBackground(true);
    ui->centralWidget->setPalette(pal);

    /*init pie char for usb*/
    drawPieChartInit();
    initTimer();
#if 0
    /*add scroll bar*/
    QScrollArea *scroll = new QScrollArea(ui->centralWidget);
    scroll->setWidget(ui->widget);
    ui->widget->setMinimumSize(1500,1000);
    ui->horizontalLayout_5->addWidget(scroll);
#endif
    /*create searchThread*/
    searchThread = new SearchThread(this);
    connect(searchThread, SIGNAL(finished()), searchThread, SLOT(deleteLater()));
    connect(searchThread, SIGNAL(sendUnmountNum(int)), this, SLOT(slotCloseDev(int)));
    connect(searchThread, SIGNAL(sendMountNum(char *)), this, SLOT(slotFindDev(char *)));
    connect(searchThread, SIGNAL(starCountingDown()), this, SLOT(starFtpTime()));

    searchThread->start();

    /*创建FTP线程*/
    ftpThread = new QThread(this);
    ftpWork = new FtpManager();

    connect(ftpThread, SIGNAL(finished()), ftpThread, SLOT(deleteLater()));
    connect(this, SIGNAL(starFtpTransmission()), ftpWork, SLOT(transmission_task()));

    ftpWork->moveToThread(ftpThread);

    ftpThread->start();
}

void MainWindow::initTimer()
{
    /*timer 用来更新本地存储界面*/
    timer = new QTimer(this);

    connect(timer, SIGNAL(timeout()), this, SLOT(showLocalStorage()));
    timer->start(10000);

    /*timer_ftp用来进行倒计时开始FTP上传文件*/
    timer_ftp = new QTimer(this);

    connect(timer_ftp, SIGNAL(timeout()), this, SLOT(emitToFtpTranslation()));
}

QGroupBox* MainWindow::groupBox(int i)
{
    if((i < 0) || (i > USB_MAX_NUM))
    {
        return nullptr;
    }

    switch (i)
    {
        case 0:
            return ui->groupBox;
        case 1:
            return ui->groupBox_2;
        case 2:
            return ui->groupBox_3;
        case 3:
            return ui->groupBox_4;
        case 4:
            return ui->groupBox_5;
        case 5:
            return ui->groupBox_6;
        case 6:
            return ui->groupBox_7;
        case 7:
            return ui->groupBox_8;
        case 8:
            return ui->groupBox_9;
        case 9:
            return ui->groupBox_10;
        case 10:
            return ui->groupBox_11;
        case 11:
            return ui->groupBox_12;
        case 12:
            return ui->groupBox_13;
        case 13:
            return ui->groupBox_14;
        case 14:
            return ui->groupBox_15;
        case 15:
            return ui->groupBox_16;
        default:
            return nullptr;
    }

    return nullptr;
}
void MainWindow::drawPieChartInit()
{
    int i;
    QGroupBox *tmpGroup = nullptr;

    local.slice_1 = new QPieSlice(QStringLiteral("free"), 1, this);
    local.slice_1->setBrush(Qt::darkGray);
    local.slice_2 = new QPieSlice(QStringLiteral("used"), 0, this);

    // 将两个饼状分区加入series
    local.series = new QPieSeries(this);
    local.series->append(local.slice_1);
    local.series->append(local.slice_2);
    local.series->setPieSize(0.8);

    local.chart = new QChart();
    local.chart->addSeries(local.series);

    local.chartview = new QChartView(this);
    local.chartview->show();
    local.chartview->setChart(local.chart);
    local.chartview->setRenderHint(QPainter::Antialiasing);
    local.chartview->setAutoFillBackground(true);

    local.verticalLayout_local = new QVBoxLayout(ui->groupBox_17);
    local.verticalLayout_local->setSpacing(0);
    local.verticalLayout_local->setContentsMargins(0, 0, 0, 0);
    local.verticalLayout_local->addWidget(local.chartview);

    local.label1 = new QLabel(tmpGroup);
    local.label1->setText(QApplication::translate("MainWindow", "总容量:", Q_NULLPTR));

    local.label2 = new QLabel(tmpGroup);
    local.label2->setAlignment(Qt::AlignLeft);

    local.label3 = new QLabel(tmpGroup);
    local.label3->setText(QApplication::translate("MainWindow", "已用空间:", Q_NULLPTR));

    local.label4 = new QLabel(tmpGroup);
    local.label4->setAlignment(Qt::AlignLeft);

    local.horizontalLayout_local = new QHBoxLayout();
    local.horizontalLayout_local->setSpacing(2);
    local.horizontalLayout_local->setContentsMargins(0, 0, 0, 0);
    local.horizontalLayout_local->addWidget(local.label1);
    local.horizontalLayout_local->addWidget(local.label2);
   // local.horizontalSpacer_local = new QSpacerItem(10, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
 //   local.horizontalLayout_local->addItem(local.horizontalSpacer_local);

    local.horizontalLayout_local->addWidget(local.label3);
    local.horizontalLayout_local->addWidget(local.label4);

   local.verticalLayout_local->addLayout(local.horizontalLayout_local);

    for(i = 0; i < USB_MAX_NUM; i++)
    {
        usb[i].clearFlag  = false;

        tmpGroup = groupBox(i);
        if(tmpGroup == nullptr)
            return;

        usb[i].slice_1 = new QPieSlice(QStringLiteral("free"), 1, this);
        usb[i].slice_1->setBrush(Qt::darkGray);
        usb[i].slice_2 = new QPieSlice(QStringLiteral("used"), 0, this);

        // 将两个饼状分区加入series
        usb[i].series = new QPieSeries(this);
        usb[i].series->append(usb[i].slice_1);
        usb[i].series->append(usb[i].slice_2);
        usb[i].series->setPieSize(0.6);

        usb[i].chart = new QChart();
        usb[i].chart->addSeries(usb[i].series);

        usb[i].chartview = new QChartView(this);
        usb[i].chartview->show();
        usb[i].chartview->setChart(usb[i].chart);
        usb[i].chartview->setRenderHint(QPainter::Antialiasing);
        usb[i].chartview->setAutoFillBackground(true);

        usb[i].verticalLayout_1 = new QVBoxLayout(tmpGroup);
        usb[i].verticalLayout_1->setSpacing(0);
        usb[i].verticalLayout_1->setContentsMargins(0, 0, 0, 0);
        usb[i].verticalLayout_1->addWidget(usb[i].chartview);

        usb[i].label1 = new QLabel(tmpGroup);
        usb[i].label1->setText(QApplication::translate("MainWindow", "总容量:", Q_NULLPTR));

        usb[i].label2 = new QLabel(tmpGroup);
        usb[i].label2->setAlignment(Qt::AlignLeft);

        usb[i].label3 = new QLabel(tmpGroup);
        usb[i].label3->setText(QApplication::translate("MainWindow", "已用空间:", Q_NULLPTR));

        usb[i].label4 = new QLabel(tmpGroup);
        usb[i].label4->setAlignment(Qt::AlignLeft);

        usb[i].horizontalLayout_1 = new QHBoxLayout();
        usb[i].horizontalLayout_1->setSpacing(2);
        usb[i].horizontalLayout_1->setContentsMargins(0, 0, 0, 0);
        usb[i].horizontalLayout_1->addWidget(usb[i].label1);
        usb[i].horizontalLayout_1->addWidget(usb[i].label2);
        usb[i].horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
        usb[i].horizontalLayout_1->addItem(usb[i].horizontalSpacer);

        usb[i].horizontalLayout_1->addWidget(usb[i].label3);
        usb[i].horizontalLayout_1->addWidget(usb[i].label4);

        usb[i].horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
        usb[i].horizontalLayout_1->addItem(usb[i].horizontalSpacer_2);
        usb[i].verticalLayout_1->addLayout(usb[i].horizontalLayout_1);

        usb[i].label5 = new QLabel(tmpGroup);
        usb[i].label5->setText(QApplication::translate("MainWindow", "速度:", Q_NULLPTR));
        usb[i].label6 = new QLabel(tmpGroup);
        usb[i].label6->setAlignment(Qt::AlignLeft);
        usb[i].label6->setMinimumWidth(80);

        usb[i].horizontalLayout_2 = new QHBoxLayout();
        usb[i].horizontalLayout_2->setSpacing(2);
        usb[i].horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        usb[i].horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        usb[i].horizontalLayout_2->addWidget(usb[i].label5);
        usb[i].horizontalLayout_2->addWidget(usb[i].label6);

        usb[i].progressBar = new QProgressBar(tmpGroup);
        usb[i].progressBar->setRange(0,100);
        usb[i].horizontalLayout_2->addWidget(usb[i].progressBar);

        usb[i].verticalLayout_1->addLayout(usb[i].horizontalLayout_2);
    }
}
