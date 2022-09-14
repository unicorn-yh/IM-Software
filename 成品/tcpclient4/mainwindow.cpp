#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "addfriend.h"
# include"delfriend.h"
#include "localdb.h"
#include "clientfile.h"
#include "settingdialog.h"
#include <QApplication>
#include <QListWidget>
#include <QListWidgetItem>
#include <QMessageBox>
#include<QHostInfo>
#include<QScrollBar>
#include<QDateTime>
#include<QNetworkInterface>
#include<QProcess>
#include <QDebug>
#include<widget.h>
#include "logindialog.h"

 int hosthost=8899;
QString MainWindow::connectport="";
//数据库处理类对象
LocalDB *dbPro = new LocalDB();

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    (this)->setWindowTitle(LoginDialog::usrname+tr(" 主窗口"));

    //创建监听UDP套接字并初始化
    udpSocket = new QUdpSocket(this);
    tcpSocket=new  QTcpSocket(this);
    tcpSocket2=new  QTcpSocket(this);
    //创建发送信息UDP套接字并初始化，向指定ip的用户发送信息
    msgUdpSocket = new QUdpSocket(this);

}

MainWindow::~MainWindow()
{
    delete ui;

}

bool MainWindow::connectTofriend(QString peerID)
{
    QString hostip="127.0.0.1";
    int hosthost=8899;
    tcpSocket2->abort();
    tcpSocket2->connectToHost(hostip, hosthost);//链接服务器
    if(!tcpSocket2->waitForConnected(30000))
    {
        QMessageBox::warning(this, "Warning!", "网络错误", QMessageBox::Yes);
    }
    QString loginmessage = QString("checkuserdata##%1").arg(peerID);
    tcpSocket2->write(loginmessage.toUtf8());
    tcpSocket2->flush();
    connect(tcpSocket2,&QTcpSocket::readyRead,[=](){
        QByteArray buffer = tcpSocket2->readAll();
        MainWindow::connectport=QString(buffer);
        qDebug()<<MainWindow::connectport;
        qDebug()<<"friend port is"+MainWindow::connectport+" my port is "+ LoginDialog::myport;


    });
        connect(udpSocket, SIGNAL(readyRead()), this, SLOT(recvData()));
        return true;
}

void MainWindow:: recvData(){
    while(udpSocket->hasPendingDatagrams())
        {
            QByteArray datagram;
            datagram.resize(udpSocket->pendingDatagramSize());

            udpSocket->readDatagram(datagram.data(), datagram.size());

            QString msg = datagram.data();
            ui->msgBrowser->setTextColor(Qt::blue);
            ui->msgBrowser->setCurrentFont(QFont("Times New Roman", 12));
            ui->msgBrowser->append(peerID);
            ui->msgBrowser->setCurrentFont(QFont("Times New Roman", 15));
            ui->msgBrowser->append(msg);

        }
}

void MainWindow::sendMessage()
{
    qDebug()<<MainWindow::connectport;
    QString sendStr =ui->msgEdit->toPlainText();
    if(sendStr=="")
    {
        QMessageBox::warning(0,tr("警告"),
                             tr("发送的内容不能为空"),QMessageBox::Ok);
        return;
    }
    QByteArray datagram;
    datagram.resize(udpSocket->pendingDatagramSize());
    int length = udpSocket->writeDatagram(
                        QVariant(sendStr).toByteArray(),
                        QHostAddress::AnyIPv4,
        MainWindow::connectport.toInt());
    ui->msgBrowser->setTextColor(Qt::blue);
    ui->msgBrowser->setCurrentFont(QFont("Times New Roman", 12));
    ui->msgBrowser->append(LoginDialog::usrname);
    ui->msgBrowser->setCurrentFont(QFont("Times New Roman", 15));
    ui->msgBrowser->append(sendStr);
            ui->msgEdit->clear();
            ui->msgEdit->setFocus();

}

void MainWindow::startChat(QString peerID)
{
    //双击好友列表进入后，新开一个聊天界面
    ui->msgBrowser->clear();
    qDebug()<<peerID;
    connectTofriend(peerID);

}

void MainWindow::on_delFriButton_clicked(bool)
{
    delfriend *del=new delfriend(this);
    del->show();
    //下面写数据库删除函数
}

void MainWindow::on_addFriButton_clicked(bool)
{
    addfriend *add=new addfriend(this);
    add->show();
    //下面写数据库新增函数
}

void MainWindow::on_sendButton_clicked()
{
    sendMessage();
}

void MainWindow::on_friendList_itemDoubleClicked(QListWidgetItem *item)
{
    item->setBackground(QColor('aqua'));    //恢复默认颜色
    peerID=item->text();
    startChat(item->text());
}

void MainWindow::on_refreshButton_clicked()
{
    udpSocket->abort();
    bool bindResult = udpSocket->bind(QHostAddress::AnyIPv4,LoginDialog::myport.toInt());
    qDebug()<<"connect "+QString(LoginDialog::myport) ;
    if(!bindResult)
    {
          QMessageBox::warning(this, tr("Waring"),
                                    tr("监听端口失败"), QMessageBox::Yes);
          return ;
    }
    else
    {
          QMessageBox::warning(NULL,tr("note"),tr("开始监听端口"));
     }
    ui->friendList->clear();
    tcpSocket->abort();
    tcpSocket->connectToHost("127.0.0.1", 8899);
    if(!tcpSocket->waitForConnected(30000))
    {
        QMessageBox::warning(this, "Warning!", "网络错误", QMessageBox::Yes);
    }
    QString loginmessage = QString("checkfriendlist##%1").arg(LoginDialog::usrname);
    tcpSocket->write(loginmessage.toUtf8());
    tcpSocket->flush();
    connect(tcpSocket,&QTcpSocket::readyRead,[=](){
        QByteArray buffer = tcpSocket->readAll();
        qDebug()<< QString (buffer);
        QStringList sections = QString(buffer).split('#');
        for(int i=0;i<sections.size();i++){
            if(sections[i]!=""){
                QListWidgetItem *b=new QListWidgetItem(sections[i]);
                ui->friendList->addItem(b);
                b->setBackground(QColor('aqua'));
            }

        }
        return;
    });
}


void MainWindow::on_sendFileButton_clicked()
{
    clientfile c;
    c.exec();
}


void MainWindow::on_setButton_clicked()
{
    settingDialog c;
    c.exec();
}


void MainWindow::on_freeTalkButton_clicked()
{
    Widget *c=new Widget(LoginDialog::usrname);
    c->show();
}

void MainWindow::on_clearButton_clicked()
{
    ui->msgEdit->clear();
}

