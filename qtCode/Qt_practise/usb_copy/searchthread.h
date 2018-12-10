#ifndef SEARCHTHREAD_H
#define SEARCHTHREAD_H

#include <QThread>

class MainWindow;
class SearchThread : public QThread
{
    Q_OBJECT
public:
    explicit SearchThread(QObject *parent);
    ~SearchThread();
protected:
    void run();

private:
    void read_mount_point(void);
    void read_unmount_point(void);
    void check_ftp_transmission(void);

public:
    char mountPoint[20];

signals:
    void sendUnmountNum(int);
    void sendMountNum(char *);
    //开始倒计时
    void starCountingDown();
};

#endif // SEARCHTHREAD_H
