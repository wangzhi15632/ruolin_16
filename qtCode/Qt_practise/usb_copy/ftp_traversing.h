#ifndef FTP_TRAVERSING_H
#define FTP_TRAVERSING_H

#include <QString>
#include <QObject>
#include <time.h>
#include "usb_copy.h"

class FtpTraversing: public QObject
{
    Q_OBJECT

public:
    explicit FtpTraversing();

private:
    //char* make_path(char *dest, const char *frt, const char *snd);
    //遍历汇总
  //  int walk_sum(const char* path_from, const char* path_tree);
   // int sum_up(const char* path_tree, const struct stat* st);
    //遍历传输
    //int walk_transmisson(const char* path_from, const char* path_tree);
    //int transmission_action(const char* path_from, const char* path_tree, const struct stat* st);

signals:
   // void starFtpPut(char *, const QString, long long);

private slots:
   // void transmission_task();

private:

    QString ftpDir;

};
#endif // FTP_TRAVERSING_H
