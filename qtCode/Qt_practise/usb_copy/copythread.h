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
    CopyThread(MainWindow* window);

protected:
   void run();
private:
    int dir_is_empty(char* dirname);
    char* human_time(time_t t, char *text);
    void print_message(int t, const char* fmt, ...);
    char* make_path(char *dest, const char *frt, const char *snd);
    void show_status(BOOL finish);
    int walk(const char* path_from, const char* path_to, const char* path_tree, each_opp_t opp);
    bool is_self_copy(const char* src, const char* dest);
    void install_time();
    int cp_task(char *dir);

    friend int sum_up(const char* path_from, const char* path_to, const char* path_tree, const struct stat* st, CopyThread* copy);
    friend int action(const char* path_from, const char* path_to, const char* path_tree, const struct stat* st, CopyThread* copy);

private slots:
   void rcvPar(char *);
   void timeOutEmit(void);

signals:
    void sendToUI(int, sum_t, copied_t, time_t, bool);

private:
   char mountDir[20];
   bool rcvFlag;
   sum_t sum;
   copied_t copied;
   time_t copy_start_time;
   int num;

   MainWindow *wind;

signals:
    void sendUDevInfo(int, unsigned long, unsigned long, unsigned long);
};


#endif // COPYTHREAD_H
