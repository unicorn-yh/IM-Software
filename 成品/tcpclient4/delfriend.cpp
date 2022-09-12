#include "delfriend.h"
#include "ui_delfriend.h"

delfriend::delfriend(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::delfriend)
{
    ui->setupUi(this);
     tcpSocket = new QTcpSocket();
}

delfriend::~delfriend()
{
    delete ui;
}

void delfriend::on_buttonBox_accepted()
{
    QString name=ui->name->text();
    tcpSocket->abort();
    tcpSocket->connectToHost("127.0.0.1", 8899);
    if(!tcpSocket->waitForConnected(30000))
    {
        QMessageBox::warning(this, "Warning!", "网络错误", QMessageBox::Yes);
    }
    QString loginmessage = QString("delfriend##%1##%2").arg(LoginDialog::usrname,name);
    tcpSocket->write(loginmessage.toUtf8());
    tcpSocket->flush();
  /*  connect(tcpSocket,&QTcpSocket::readyRead,[=](){
        QByteArray buffer = tcpSocket->readAll();
        qDebug()<<QString(buffer);
        if(QString(buffer)==QString("successed"))
        {
            QMessageBox::information(this, "Note", "删除好友成功", QMessageBox::Yes);
        }
        else if (QString(buffer)==QString("failed")){
            QMessageBox::warning(this, "Warning!", "删除失败", QMessageBox::Yes);
        }

    });*/
    accept();
}

