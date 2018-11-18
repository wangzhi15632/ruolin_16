#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QtCharts>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include "usb_copy.h"

#define USB_MAX_NUM 16

typedef struct
{
    QPieSlice *slice_1, *slice_2;
    QPieSeries *series;
    QChart *chart;
    QChartView *chartview;
    QVBoxLayout *verticalLayout_1;
    QLabel *label1, *label2, *label3, *label4, *label5, *label6;
    QHBoxLayout *horizontalLayout_1, *horizontalLayout_2;
    QSpacerItem *horizontalSpacer, *horizontalSpacer_2, *horizontalSpacer_3;
    QProgressBar *progressBar;

}usb_t;



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
    void drawPieChartInit();
    QGroupBox* groupBox(int i);
    char* human_size(long long s, char *hs);

private:
    usb_t usb[USB_MAX_NUM];

private:
    Ui::MainWindow *ui;

private slots:
    void createDev(int);
    void slotShow(int, unsigned long, unsigned long, unsigned long);
    void slotProgress(int, sum_t, copied_t, time_t, bool);

};

#endif // MAINWINDOW_H
