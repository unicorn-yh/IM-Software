#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include <QLabel>
#include <QSqlDatabase>
#include <QFile>
#include <QTimer>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void sendFileData(); //发送文件数据


private:
    Ui::MainWindow *ui;
    QTcpServer * tcpServer;
    QTcpSocket* tcpSocket;
    QLabel   * m_status;
    QSqlDatabase db;
    QFile file;       //发送的文件对象
    QString fileName; //发送文件名
    qint64 fileSize; //文件大小
    qint64 sendSize; //已经发送文件的大小
    QTimer timer; //定时器


private slots:
    void on_fileBtn_clicked();    //服务器的上传文件按钮
    void on_sendBtn_clicked();    //服务器的发送文件按钮

};
#endif // MAINWINDOW_H
