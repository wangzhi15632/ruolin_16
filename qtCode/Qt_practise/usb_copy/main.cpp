#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    /*add log file*/
    extern void outputMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg);
    qInstallMessageHandler(outputMessage);

    MainWindow w;
    w.show();

    return a.exec();
}
