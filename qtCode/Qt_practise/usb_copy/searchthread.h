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

signals:
    void sendPar(char *);
    void sendUnmountNum(int);

public:
    MainWindow *window;

};

#endif // SEARCHTHREAD_H
