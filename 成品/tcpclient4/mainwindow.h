#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUdpSocket>
#include"QListWidgetItem"
#include<QTcpSocket>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    static QString connectport;
protected:
    void processPendinDatagrames(); //实时处理监听信号的函数
    bool connectTofriend(QString peerID);         //点击好友item时会与对方建立连接，对方不在线会作出提示
    void sendMessage();     //发送信息
    void showMessage(QString userName, QString message, QString time);
    void startChat(QString peerID);

    QString getMessage();

private slots:

    void on_delFriButton_clicked(bool checked);

    void on_addFriButton_clicked(bool checked);

    void on_sendButton_clicked();

    void on_friendList_itemDoubleClicked(QListWidgetItem *item);

    void on_refreshButton_clicked();

    void on_sendFileButton_clicked();

    void on_setButton_clicked();
    void recvData();
    void on_freeTalkButton_clicked();

    void on_clearButton_clicked();

private:
    Ui::MainWindow *ui;
    QUdpSocket *udpSocket;
    QUdpSocket *msgUdpSocket;
    QTcpSocket *tcpSocket;
    QTcpSocket *tcpSocket2;
    QString localHost = "127.0.0.1";
    QString peerID;
};

#endif // MAINWINDOW_H
