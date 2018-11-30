#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::on_uploadButton_clicked() //上传文件
{
    file = new QFile("/home/wz/test1/hiv00000.mp4");
    file->open(QIODevice::ReadOnly);
    QByteArray byte_file = file->readAll();
    QNetworkAccessManager *accessManager = new QNetworkAccessManager(this);
    accessManager->setNetworkAccessible(QNetworkAccessManager::Accessible);
    QUrl url("ftp://192.168.1.30/aaa/" + ui->lineEdit->text() + ".mp4");
    url.setPort(21);
    //url.setUserName("wz");
    //url.setPassword("123456");
    QNetworkRequest request(url);
    reply = accessManager->put(request, byte_file);
    progressBar = new QProgressBar(this);
    progressBar->setValue(0);
    progressBar->show();
    connect(accessManager,SIGNAL(finished(QNetworkReply*)),this,SLOT(replyFinished(QNetworkReply*)));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),this,SLOT(loadError(QNetworkReply::NetworkError)));
    connect(reply, SIGNAL(uploadProgress(qint64 ,qint64)), this, SLOT(loadProgress(qint64 ,qint64)));
}
void MainWindow::on_downloadButton_clicked() //下载文件
{
    file = new QFile(QApplication::applicationDirPath() + "/" + ui->lineEdit->text() + ".txt");
    file->open(QIODevice::WriteOnly);
    QNetworkAccessManager *accessManager = new QNetworkAccessManager(this);
    accessManager->setNetworkAccessible(QNetworkAccessManager::Accessible);
    QUrl url("ftp://192.168.1.30/123.txt");
    url.setPort(21);
    //url.setUserName("peng");
    //url.setPassword("123456");
    QNetworkRequest request(url);
    reply = accessManager->get(request);
    progressBar = new QProgressBar();
    progressBar->setValue(0);
    progressBar->show();
    connect((QObject *)reply, SIGNAL(readyRead()), this, SLOT(readContent()));
    connect(accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),this,SLOT(loadError(QNetworkReply::NetworkError)));
    connect(reply, SIGNAL(downloadProgress(qint64 ,qint64)), this, SLOT(loadProgress(qint64 ,qint64)));
}
void MainWindow::readContent() //下载时向本地文件中写入数据
{
    file->write(reply->readAll());
}
void MainWindow::replyFinished(QNetworkReply*) //删除指针，更新和关闭文件
{
    if(reply->error() == QNetworkReply::NoError)
    {
        reply->deleteLater();
        file->flush();
        file->close();
    }
    else
    {
        QMessageBox::critical(NULL, tr("Error"), "Failed!!!");
    }
}
void MainWindow::loadProgress(qint64 bytesSent, qint64 bytesTotal) //更新进度条
{
    qDebug() << "loaded" << bytesSent << "of" << bytesTotal;
    progressBar->setMaximum(bytesTotal);//最大值
    progressBar->setValue(bytesSent); //当前值
}
void MainWindow::loadError(QNetworkReply::NetworkError) //传输中的错误输出
{
    qDebug()<<"Error: "<<reply->error();
}
