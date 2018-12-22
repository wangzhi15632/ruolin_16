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
#include "ftp_traversing.h"

QSemaphore CopyThreadNum(USB_MAX_NUM);
bool ftpFlag = true;
QMutex mutex, ftp_mutex, copy_num_mutex;
char path_from_full[MAX_PATH_LENGTH];
sum_t ftp_sum;
copied_t ftp_transmission;
time_t ftp_transmission_start_time;
char *path[HARD_DISK_MAX_NUM] = {"/usb_copy_dir/usb_0_1", "/usb_copy_dir/usb_2_3", "/usb_copy_dir/usb_4_5", "/usb_copy_dir/usb_6_7",
                      "/usb_copy_dir/usb_8_9", "/usb_copy_dir/usb_10_11", "/usb_copy_dir/usb_12_13", "/usb_copy_dir/usb_14_15"};
/*记录目录当前正在写入的USB设备，为了保证拷贝速率，目录正在拷贝的USB设备不能大于2*/
unsigned int dir_writting_num[HARD_DISK_MAX_NUM] = {0, 0, 0, 0, 0, 0, 0, 0};
bool is_format_usb = true;

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

    delete searchThread;

    ftpThread->quit();
    ftpThread->wait();
    ftpTraverThread->quit();
    ftpTraverThread->wait();

    qDebug() << "delete ftp";
    delete ftpWork;
    delete ftpTraver;
}

void MainWindow::slotFindDev(char *mountPoint)
{
    qInfo("find a usb dev moutpoint:%s", mountPoint);

    if(timer_ftp->isActive())
    {
        qDebug() << "stop timer_ftp";
        timer_ftp->stop();
    }

    CopyThread *copyThread = new CopyThread();//can't add this parameter

    qDebug() << "copyThread addr:" << copyThread;
    connect(copyThread, SIGNAL(sendUDevInfo(int, unsigned long, unsigned long, unsigned long)), this,
            SLOT(slotShow(int, unsigned long, unsigned long, unsigned long)));

    connect(copyThread, SIGNAL(sendUDevInfoTranscoding(int, unsigned long long)), this,
            SLOT(slotShowTranscoding(int, unsigned long long)));

    connect(copyThread, SIGNAL(sendToUI(int, sum_t, copied_t, time_t, bool)), this,
            SLOT(slotProgress(int, sum_t, copied_t, time_t, bool)));
    connect(copyThread, SIGNAL(finished()), copyThread, SLOT(deleteLater()));
    connect(copyThread, SIGNAL(finished()), copyThread, SLOT(test()));

    copyThread->cp_dir(mountPoint);

    copyThread->start();
}

void MainWindow::slotCloseDev(int num)
{
    usb[num].clearFlag = true;

    usb[num].chartview->chart()->setTitleBrush(QBrush(Qt::darkGray));
    usb[num].chartview->chart()->setTitle(tr("请插入SD卡"));

    usb[num].slice_1->setValue(0);
   // usb[num].slice_1->setBrush(Qt::darkGray);
    usb[num].slice_2->setValue(1);

    usb[num].label2->clear();
    usb[num].label4->clear();
    usb[num].label6->clear();
    usb[num].progressBar->setValue(0);

    qInfo("close a usb dev %d", num);
}

void MainWindow::slotShowTranscoding(int i, unsigned long long size)
{
    char disk_avail[20];

    if(size == 0)
    {
        usb[i].chartview->chart()->setTitle(tr("SD卡未检测到数据"));
    }

    human_size(size, disk_avail);

    usb[i].label4->setText(disk_avail);

    usb[i].slice_1->setValue(1 - static_cast<double>(size)/static_cast<double>(usb[i].usb_sum));
    usb[i].slice_2->setValue(static_cast<double>(size)/static_cast<double>(usb[i].usb_sum));

}

void MainWindow::slotShow(int i, unsigned long block,unsigned long bsize,unsigned long bavail)
{
    char disk_avail[20];
    char disk_size[20];

    usb[i].clearFlag = false;

    usb[i].chartview->chart()->setTitle(tr("正在拷贝,请稍候"));
    usb[i].chartview->chart()->setTitleBrush(QBrush(Qt::blue));
    //usb[i].chartview->chart()->setTitleFont(QFont("Arial",7));

    usb[i].usb_sum = block * bsize;

    human_size(usb[i].usb_sum, disk_size);
    human_size(usb[i].usb_sum - bavail * bsize, disk_avail);

    usb[i].label2->setText(disk_size);
    usb[i].label4->setText(disk_avail);

    usb[i].slice_1->setValue((1 - static_cast<double>(bavail)/static_cast<double>(block)));
    usb[i].slice_2->setValue(static_cast<double>(bavail)/(static_cast<double>(block)));

    qInfo("draw usb %d pie", i);
}
/*显示为可读数字*/
void MainWindow::showLocalStorage()
{
    char disk_avail[20] = {0};
    char disk_size[20] = {0};
    struct statfs s[HARD_DISK_MAX_NUM];
    unsigned long long blocks = 0, free = 0;

    for(int num = 0; num < HARD_DISK_MAX_NUM; num++)
    {
        memset(&s[num], 0, sizeof(struct statfs));

        if(0 != statfs(path[num], &s[num]))
        {
            continue;
        }

        blocks += s[num].f_blocks;
        free += s[num].f_bfree;
    }

    human_size(blocks * s[0].f_bsize, disk_size);
    human_size((blocks - free) * s[0].f_bsize, disk_avail);

    local.label2->setText(disk_size);
    local.label4->setText(disk_avail);

    local.slice_1->setValue((1 - static_cast<double>(free)/static_cast<double>(blocks)));
    local.slice_2->setValue(static_cast<double>(free)/(static_cast<double>(blocks)));
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
    ui->textEdit->clear();
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

void MainWindow::slotProgress(int i, sum_t sum, copied_t copied, time_t copy_start_time, bool flag)
{
    time_t cur_time;
    int percent;
    char hs[20];
    long long sp = 0;
    char speed[20];

    if(usb[i].clearFlag == true)
        return;

    if(flag == false)
    {
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

        if(percent == 100)
        {
            usb[i].chartview->chart()->setTitleBrush(QBrush(Qt::darkRed));
            usb[i].chartview->chart()->setTitle(tr("正在格式化SD卡，请稍候"));
        }
    }
    else
    {
        usb[i].chartview->chart()->setTitleBrush(QBrush(Qt::darkRed));
        usb[i].chartview->chart()->setTitle(tr("拷贝并格式化完成，请拔出SD卡"));
    }

}
void MainWindow::updateFtpProgress(QString str, sum_t ftp_sum, copied_t ftp_transmissioned, time_t ftp_star_time)
{
    time_t cur_time;
    char hs[20];
    long long sp = 0;
    char speed[20];
    int percent;

    ui->textEdit->append(str);

    time(&cur_time);
    if(ftp_sum.size == 0)
    {
        percent = 0;
    }
    else
    {
        percent = static_cast<int>((ftp_transmissioned.size * 1.0 / ftp_sum.size) * 100);
    }

    if(cur_time > ftp_star_time)
    {
        sp = ftp_transmissioned.size / (cur_time - ftp_star_time);
        sprintf(speed, "%s/s", human_size(sp, hs));
    }
    else
    {
        sprintf(speed, "-");
    }

    ui->label_2->setText(speed);
    ui->progressBar->setValue(percent);
}

void MainWindow::ftpCfgBtnClicked()
{
    if(QDialog::Accepted == ftpCfg->exec())
    {
        ftpWork->ip = ftpCfg->getIPAddr();
        ftpWork->port = ftpCfg->getPortAddr();
        ftpWork->userName = ftpCfg->getUserName();
        ftpWork->password = ftpCfg->getPassword();

        qDebug() << "ip:%s" << ftpWork->ip;
        qDebug() << "port:%s" << ftpWork->port;
        qDebug() << "userName:%s" << ftpWork->userName;
        qDebug() << "password:%s" << ftpWork->password;

        if(ftpWork->ip != nullptr && ftpWork->port != nullptr)
        {
            ftpWork->setHostPort(ftpWork->ip, ftpWork->port.toInt());
        }

        if(ftpWork->userName != nullptr && ftpWork->password != nullptr)
        {
            ftpWork->setUserInfo(ftpWork->userName, ftpWork->password);
        }
    }
}

void MainWindow::usbFmtActClicked()
{
    if(QDialog::Accepted == usbfmt->exec())
    {
        ;
    }
}

void MainWindow::test()
{
    qDebug() <<"test";
}

void MainWindow::init()
{
    QFile *file = new QFile("/media/mount_info.txt");
    file->remove();
    QFile *file2 = new QFile("/media/unmount_info.txt");
    file2->remove();

    qDebug() << "main thread: " << QThread::currentThread();
    /*config mainWindow size and title*/
    setWindowTitle(tr("16路转储平台 V1.0"));
    setMinimumSize(QSize(800, 600));

    /*regist signal-slot parameter*/
    qRegisterMetaType<sum_t>("sum_t");
    qRegisterMetaType<copied_t>("copied_t");
    qRegisterMetaType<time_t>("time_t");

    /*config centralWidget background*/
    QPalette pal(ui->centralWidget->palette());
    pal.setColor(QPalette::Background, Qt::white);
    ui->centralWidget->setAutoFillBackground(true);
    ui->centralWidget->setPalette(pal);

    usbfmt = new usbFormat(this);

    connect(ui->action_usb_format, SIGNAL(triggered(bool)), this, SLOT(usbFmtActClicked()));

    /*init pie char for usb*/
    drawPieChartInit();

    ftpCfg = new FtpConfig(this);
    connect(ui->pushButton_ftpCfg, SIGNAL(clicked(bool)), this, SLOT(ftpCfgBtnClicked()));

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

    /*创建FTP传输线程*/
    ftpThread = new QThread(this);
    ftpWork = new FtpManager();

    connect(ftpThread, SIGNAL(finished()), ftpThread, SLOT(deleteLater()));

    connect(ftpWork, SIGNAL(sendFtpInfo(QString, sum_t, copied_t, time_t)), this,
            SLOT(updateFtpProgress(QString, sum_t, copied_t, time_t)));

    ftpWork->moveToThread(ftpThread);
    ftpThread->start();

    /*创建FTP遍历线程*/
    ftpTraverThread = new QThread(this);
    ftpTraver = new FtpTraversing();

    connect(ftpTraverThread, SIGNAL(finished()), ftpTraverThread, SLOT(deleteLater()));
    connect(this, SIGNAL(starFtpTransmission()), ftpTraver, SLOT(transmission_task()));
    connect(ftpTraver, SIGNAL(starFtpPut(char *, const QString, long long)), ftpWork, SLOT(put(char *, const QString, long long)));
    ftpTraver->moveToThread(ftpTraverThread);

    ftpTraverThread->start();
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

#if 0
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
#endif
void MainWindow::drawPieChartInit()
{
    int i;
    QGroupBox *tmpGroup = nullptr;

    local.slice_1 = new QPieSlice(QStringLiteral("used"), 0, this);
    local.slice_2 = new QPieSlice(QStringLiteral("free"), 1, this);

    //local.slice_2->setBorderColor(Qt::red);
    local.slice_2->setColor(Qt::lightGray);

    local.series = new QPieSeries(ui->groupBox_17);
    local.series->setHoleSize(0.35);
    local.series->append(local.slice_1);
    local.series->append(local.slice_2);

    local.chartview = new QChartView(ui->groupBox_17);
    local.chartview->setRenderHint(QPainter::Antialiasing);
 //   local.chartview->chart()->setTitle("DonutChart Example");
    local.chartview->chart()->addSeries(local.series);
    local.chartview->chart()->legend()->setAlignment(Qt::AlignBottom);
    local.chartview->chart()->setTheme(QChart::ChartThemeLight);
    local.chartview->chart()->legend()->setFont(QFont("Arial",7));

    local.verticalLayout_local = new QVBoxLayout(ui->groupBox_17);
    local.verticalLayout_local->setSpacing(0);
    local.verticalLayout_local->setContentsMargins(0, 0, 0, 0);
    local.verticalLayout_local->addWidget(local.chartview);


    local.label1 = new QLabel(ui->groupBox_17);
    local.label1->setText(QApplication::translate("MainWindow", "总容量:", Q_NULLPTR));

    local.label2 = new QLabel(ui->groupBox_17);
    local.label2->setAlignment(Qt::AlignLeft);

    local.label3 = new QLabel(ui->groupBox_17);
    local.label3->setText(QApplication::translate("MainWindow", "已用空间:", Q_NULLPTR));

    local.label4 = new QLabel(ui->groupBox_17);
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

        usb[i].slice_1 = new QPieSlice(QStringLiteral("used"), 0, this);
        usb[i].slice_2 = new QPieSlice(QStringLiteral("free"), 1, this);
        usb[i].slice_2->setColor(Qt::lightGray);

        usb[i].series = new QPieSeries(tmpGroup);
        usb[i].series->setHoleSize(0.35);
        usb[i].series->append(usb[i].slice_1);
        usb[i].series->append(usb[i].slice_2);

        usb[i].chartview = new QChartView(tmpGroup);
        usb[i].chartview->setRenderHint(QPainter::Antialiasing);
        //usb[i].chartview->chart()->setTitle("DonutChart Example");
        usb[i].chartview->chart()->addSeries(usb[i].series);
        usb[i].chartview->chart()->legend()->setAlignment(Qt::AlignBottom);
        usb[i].chartview->chart()->setTheme(QChart::ChartThemeLight);
        usb[i].chartview->chart()->legend()->setFont(QFont("Arial",7));
        usb[i].chartview->chart()->setTitleBrush(QBrush(Qt::darkGray));
        usb[i].chartview->chart()->setTitle(tr("请插入SD卡"));

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
       // usb[i].horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
        //usb[i].horizontalLayout_1->addItem(usb[i].horizontalSpacer);

        usb[i].horizontalLayout_1->addWidget(usb[i].label3);
        usb[i].horizontalLayout_1->addWidget(usb[i].label4);

       // usb[i].horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
       // usb[i].horizontalLayout_1->addItem(usb[i].horizontalSpacer_2);
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

#if 0

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
#endif
}
