#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "usb_main.h"
#include <pthread.h>
#include "searchthread.h"

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

void MainWindow::slotCloseDev(int num)
{
    //qDebug("window:%d", num);
    usb[num].clearFlag = true;

    usb[num].slice_1->setValue(1);
    usb[num].slice_1->setBrush(Qt::darkGray);
    usb[num].slice_2->setValue(0);


    usb[num].label2->clear();
    usb[num].label4->clear();
    usb[num].label6->clear();
    usb[num].progressBar->setValue(0);
}

void MainWindow::slotShow(int i, unsigned long block,unsigned long bsize,unsigned long bavail)
{
    //double disk_size;
    char disk_avail[20];
    char disk_size[20];
    //qDebug("slot show:%d", i);
    usb[i].clearFlag = false;

    human_size(block * bsize, disk_size);
    human_size(block * bsize - bavail * bsize, disk_avail);

    //disk_size = block * bsize /1024/1024/1024;
    //disk_avail = bavail * bsize /1024/1024/1024;

    //usb[i].label2->setText(QString::number(disk_size)+"GB");
    //usb[i].label4->setText(QString::number(disk_size - disk_avail)+"GB");
    usb[i].label2->setText(disk_size);
    usb[i].label4->setText(disk_avail);

    usb[i].slice_1->setValue(static_cast<double>(bavail)/(static_cast<double>(block)));
    usb[i].slice_1->setBrush(Qt::lightGray);
    usb[i].slice_2->setValue((1-  static_cast<double>(bavail)/static_cast<double>(block)));
    usb[i].slice_2->setBrush(Qt::blue);

}
/*显示为可读数字*/
char* MainWindow::human_size(long long s, char *hs)
{
    if(s >= GBYTES)
    {
        sprintf(hs, "%.2fGB", (s * 1.0) / GBYTES);
    }
    else if(s >= 1024 * 1024)
    {
        sprintf(hs, "%.2fMB", (s * 1.0) / MBYTES);
    }
    else if(s > 1024)
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

    //qDebug("slot progress:%d", i);
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
    setWindowTitle(tr("16路转储平台 V1.0"));
    setMinimumSize(QSize(1400, 1000));

    QPalette pal(ui->centralWidget->palette());
    pal.setColor(QPalette::Background, Qt::white);
    ui->centralWidget->setAutoFillBackground(true);
    ui->centralWidget->setPalette(pal);

    drawPieChartInit();

    SearchThread *searchThread = new SearchThread(this);
    connect(searchThread, SIGNAL(finished()), searchThread, SLOT(deleteLater()));
    connect(searchThread, SIGNAL(sendUnmountNum(int)), this, SLOT(slotCloseDev(int)));

    searchThread->start();
}

QGroupBox* MainWindow::groupBox(int i)
{
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
    }
}
void MainWindow::drawPieChartInit()
{
    int i;
    QGroupBox *tmpGroup;
    for(i = 0; i < USB_MAX_NUM; i++)
    {
        usb[i].clearFlag  = false;

        tmpGroup = groupBox(i);
        // 构造两个饼状分区，A数据显示绿色占60%，B数据显示蓝色占40%
        usb[i].slice_1 = new QPieSlice(QStringLiteral("free"), 1, this);
        usb[i].slice_1->setBrush(Qt::darkGray);
        usb[i].slice_2 = new QPieSlice(QStringLiteral("used"), 0, this);

        // 将两个饼状分区加入series
        usb[i].series = new QPieSeries(this);
        usb[i].series->append(usb[i].slice_1);
        usb[i].series->append(usb[i].slice_2);

        usb[i].chart = new QChart();
        usb[i].chart->addSeries(usb[i].series);

        usb[i].chartview = new QChartView(this);
        usb[i].chartview->show();
        usb[i].chartview->setChart(usb[i].chart);
        usb[i].chartview->setRenderHint(QPainter::Antialiasing);

        usb[i].verticalLayout_1 = new QVBoxLayout(tmpGroup);
        usb[i].verticalLayout_1->setSpacing(2);
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
        //usb[i].label6->

        usb[i].horizontalLayout_2 = new QHBoxLayout();
        usb[i].horizontalLayout_2->setSpacing(2);
        usb[i].horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        usb[i].horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        usb[i].horizontalLayout_2->addWidget(usb[i].label5);
        usb[i].horizontalLayout_2->addWidget(usb[i].label6);
        //usb[i].horizontalSpacer_3 = new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Minimum);
       // usb[i].horizontalLayout_2->addItem(usb[i].horizontalSpacer_3);

        usb[i].progressBar = new QProgressBar();
        usb[i].progressBar->setRange(0,100);
        usb[i].horizontalLayout_2->addWidget(usb[i].progressBar);


        usb[i].verticalLayout_1->addLayout(usb[i].horizontalLayout_2);
    }
}
