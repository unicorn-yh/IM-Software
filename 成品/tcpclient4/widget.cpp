#include "widget.h"
#include "ui_widget.h"

#include<QHostInfo>
#include<QMessageBox>
#include<QScrollBar>
#include<QDateTime>
#include<QNetworkInterface>
#include<QProcess>

Widget::Widget(QString niName, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    defaultName = niName;
    ui->setupUi(this);
    ui->niNameLineEdit->setPlaceholderText(" 请输入您的昵称");

    //创建UDP套接字并初始化，调用sendMessage()函数来广播用户登录信息
    udpSocket = new QUdpSocket(this);
    port = 9999;
    udpSocket->bind(port,QUdpSocket::ShareAddress|QUdpSocket::ReuseAddressHint);
    connect(udpSocket, SIGNAL(readyRead()), this, SLOT(processPendinDatagrames()));
    sendMessage(NewParticipant);
}

Widget::~Widget()
{
    delete ui;
}

//sendMessage()函数定义代码
void Widget::sendMessage(MessageType type, QString serverAdress)
{
    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    QString localHostName = QHostInfo::localHostName();
    QString adress = getIP();
    out<<type<<getUserName()<<localHostName;

    switch (type) {
    case Message:
        if(ui->messageTextEdit->toPlainText()==""){
            QMessageBox::warning(0,tr("警告"),
                                 tr("发送的内容不能为空"),QMessageBox::Ok);
            return;
        }
        out<<adress<<getMessage();
        ui->messageBrowser->verticalScrollBar()
                ->setValue(ui->messageBrowser->verticalScrollBar()->maximum());
        break;

    case NewParticipant:
        out<<adress;
        break;

    case ParticipantLeft:
        break;

    }
    udpSocket->writeDatagram(data, data.length(), QHostAddress::Broadcast, port);
}


//程序同时需要接收UDP广播发来的数据，实现该功能的槽函数processPendinDatagrames()定义
//有数据进来时都会触发这个槽函数
void Widget::processPendinDatagrames()
{
    while (udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(datagram.data(), datagram.size());
        QDataStream in(&datagram, QIODevice::ReadOnly);
        int messageType ;
        in>>messageType;

        QString userName, localHostName, ipAdress, message;
        QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");

        switch (messageType)
        {
        case Message:
            in>>userName>>localHostName>>ipAdress>>message;
            ui->messageBrowser->setTextColor(Qt::blue);
            ui->messageBrowser->setCurrentFont(QFont("Times New Roman", 8));
            ui->messageBrowser->append("["+userName+"]"+time);
            ui->messageBrowser->append(message);
            break;

        case NewParticipant:
            in>>userName>>localHostName>>ipAdress;
            newParticipant(userName, localHostName, ipAdress);
            break;

        case ParticipantLeft:
            in>>userName>>localHostName;
            participantLeft(userName, localHostName, time);
            break;

        }
    }
}


//新用户进来的函数newParticipant()的定义
void Widget::newParticipant(QString userName, QString localHostName, QString ipAdress)
{
    bool isEmpty = ui->userTableWidget
            ->findItems(userName, Qt::MatchExactly).isEmpty();

    if(isEmpty)
    {
        QTableWidgetItem *user = new QTableWidgetItem(userName);
        QTableWidgetItem *host = new QTableWidgetItem(localHostName);
        QTableWidgetItem *ip = new QTableWidgetItem(ipAdress);

        ui->userTableWidget->insertRow(0);
        ui->userTableWidget->setItem(0,0,user);
        ui->userTableWidget->setItem(0,1,host);
        ui->userTableWidget->setItem(0,2,ip);

        ui->messageBrowser->setTextColor(Qt::gray);
        ui->messageBrowser->setCurrentFont(QFont("Times New Roman", 10));
        ui->messageBrowser->append(tr("%1在线！").arg(userName));

        ui->userNumLabel->setText(tr("在线人数：%1").arg(ui->userTableWidget->rowCount()));

        sendMessage(NewParticipant);
    }
}

//用户离开的函数participantLeft()的定义
void Widget::participantLeft(QString userName, QString localHostName, QString time)
{
    int rowNum = ui->userTableWidget->findItems(localHostName, Qt::MatchExactly).first()->row();

    ui->userTableWidget->removeRow(rowNum);
    ui->messageBrowser->setTextColor(Qt::gray);
    ui->messageBrowser->setCurrentFont(QFont("Times New Roman", 10));
    ui->messageBrowser->append(tr("%1于%2离开").arg(userName).arg(time));
    ui->userNumLabel->setText(tr("在线人数：%1")
            .arg(ui->userTableWidget->rowCount()));
}

//获取ip地址
QString Widget::getIP()
{
    QList<QHostAddress>list = QNetworkInterface::allAddresses();
    foreach(QHostAddress adress, list){
        if(adress.protocol() == QAbstractSocket::IPv4Protocol){
            return adress.toString();
        }
    }
    return 0;
}

//获取用户名
QString Widget::getUserName()
{
    if(ui->niNameLineEdit->text() != "")
    {
        return ui->niNameLineEdit->text();
    }

    return defaultName;
}


//获取输入的聊天信息
QString Widget::getMessage()
{
    QString msg = ui->messageTextEdit->toHtml();
    ui->messageTextEdit->clear();
    ui->messageTextEdit->setFocus();
    return msg;
}

//设置默认昵称
void Widget::setDefaultName(QString name)
{
    this->defaultName = name;
}


void Widget::on_sendButton_clicked()
{
    sendMessage(Message);
}

void Widget::on_exitButton_clicked()
{
    close();
}

void Widget::on_fontComboBox_currentFontChanged(const QFont &f)
{
    ui->messageTextEdit->setCurrentFont(f);
    ui->messageTextEdit->setFocus();
}

void Widget::on_italicToolBtn_clicked(bool checked)
{
    ui->messageTextEdit->setFontItalic(checked);
    ui->messageTextEdit->setFocus();
}

void Widget::on_sizeComboBox_activated(const QString &arg1)
{
    ui->messageTextEdit->setFontPointSize(arg1.toDouble());
    ui->messageTextEdit->setFocus();
}

void Widget::on_boldToolBtn_clicked(bool checked)
{
    if(checked)
        ui->messageTextEdit->setFontWeight(QFont::Bold);
    else
        ui->messageTextEdit->setFontWeight(QFont::Normal);
    ui->messageTextEdit->setFocus();
}

void Widget::on_colorToolBtn_clicked()
{
    color = QColorDialog::getColor(color, this);
    if(color.isValid()){
        ui->messageTextEdit->setTextColor(color);
        ui->messageTextEdit->setFocus();
    }
}



