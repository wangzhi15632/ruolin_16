#ifndef COPYTHREAD_H
#define COPYTHREAD_H

#include <QThread>
#include <usb_copy.h>
#include "mainwindow.h"

class MainWindow;

class CopyThread : public QThread
{
    Q_OBJECT

public:
    CopyThread();
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

private slots:
   void test();

signals:
    void sendToUI(int, sum_t, copied_t, time_t, bool);
    void sendUDevInfo(int, unsigned long, unsigned long, unsigned long);

public:
    char mountDir[20];
    bool rcvFlag;

private:
   sum_t sum;
   copied_t copied;
   time_t copy_start_time;
   int num;

};


#endif // COPYTHREAD_H
