#ifndef COPYTHREAD_H
#define COPYTHREAD_H

#include <QThread>
#include "usb_copy.h"
#include "lib/libstorage.h"

#include "mainwindow.h"

class MainWindow;

typedef struct file_name
{
    char sTrainNum[32];           /*列车号*/
    char sCarriageNum[32];        /*车厢号*/
    char sDriverNum[32];          /*司机号*/
    char sYear[5];
    char sMonth[3];
    char sDay[3];
    char sHour[3];
    char sMinutes[3];
    char sSeconds[3];

}file_name_t;

class CopyThread : public QThread
{
    Q_OBJECT

public:
    explicit CopyThread();

    void cp_dir(char *);

protected:
   void run();
private:
    int dir_is_empty(char* dirname);
    char* human_time(time_t t, char *text);
    char* make_path(char *dest, const char *frt, const char *snd);
    int walk_sum(const char* path_from, const char* path_to, const char* path_tree);
    int walk_copy(const char* path_from, const char* path_to, const char* path_tree);
    bool is_self_copy(const char* src, const char* dest);
    void install_time();
    int cp_task(char *dir);
    int sum_up(const char* path_tree, const struct stat* st);
    int action(const char* path_from, const char* path_to, const char* path_tree, const struct stat* st);
    void is_transcoding();
    void transcoding_sum();
    void transcoding_copy(const char *path_to);
private slots:
   void test();

signals:
    void sendToUI(int, sum_t, copied_t, time_t, bool);
    void sendUDevInfo(int, unsigned long, unsigned long, unsigned long);

public:
    char mountDir[20];
    bool rcvFlag;
    bool transcodingFlag;

private:
   sum_t sum;
   copied_t copied;
   time_t copy_start_time;
   int num;
};


#endif // COPYTHREAD_H
