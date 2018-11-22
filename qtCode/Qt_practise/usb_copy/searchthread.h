#ifndef SEARCHTHREAD_H
#define SEARCHTHREAD_H

#include <QThread>

class MainWindow;
class SearchThread : public QThread
{
    Q_OBJECT
public:
    SearchThread(QObject *parent);

protected:
    void run();

private:
    void read_mount_point(void);
    void read_unmount_point(void);

public:
    char mountPoint[20];

signals:
    void sendUnmountNum(int);
    void sendMountNum(char *);

};

#endif // SEARCHTHREAD_H
