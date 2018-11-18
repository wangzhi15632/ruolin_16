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

signals:
    void sendDev(char *);

public:
    MainWindow *window;

};

#endif // SEARCHTHREAD_H
