#ifndef COPYTHREAD_H
#define COPYTHREAD_H

#include <QThread>
#include "usb_copy.h"
#include "lib/libstorage.h"
#include "mainwindow.h"

#define FILE_MAX_NUM   1000
#define FILE_MAX_NAME   64
#define HARD_DISK_MAX_NUM 8

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
    int search_dir_to_write(char *dir, unsigned int i);
    bool is_dir_larger(char *dir);
    bool is_dir_writting_num(unsigned int i);
    void dir_full_and_delete(char *dir, char *dir_tree);
    int delete_min_time_dir(char *dir_del);


private slots:
   void test();

signals:
    void sendToUI(int, sum_t, copied_t, time_t, bool);
    void sendUDevInfo(int, unsigned long, unsigned long, unsigned long);
    void sendUDevInfoTranscoding(int, unsigned long long);

public:
    char mountDir[20];
    bool rcvFlag;
    bool transcodingFlag;
    unsigned long long usb_size;   /*当前U盘已用空间*/

private:
   sum_t sum;
   copied_t copied;
   time_t copy_start_time;
   int num;
   char file_name[FILE_MAX_NUM][FILE_MAX_NAME];
   int file_num;
};


#endif // COPYTHREAD_H
