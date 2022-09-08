#include "changephone.h"
#include "ui_changephone.h"
#include <QMessageBox>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QDebug>
#include "logindialog.h"

changePhone::changePhone(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::changePhone)
{
    ui->setupUi(this);
    tcpSocket = new QTcpSocket();
}

changePhone::~changePhone()
{
    delete ui;
}

void changePhone::on_doneBtn4_clicked()
{
    QString phone_num = ui->newPhone->text();
    int size = phone_num.size();

    if (size!=11 || !(phone_num.startsWith('1'))){  //检查手机号码格式
        qDebug() << "更换手机号码失败！";
        QMessageBox::warning(this, tr("警告"), tr("手机号码格式错误！"), QMessageBox::Yes);
        return;
    }

    for (int i=0;i<size;i++){
        if(!phone_num[i].isDigit()){        //手机号码只能包含数字
            qDebug() << "更换手机号码失败！";
            QMessageBox::warning(this, tr("警告"), tr("手机号码格式错误！"), QMessageBox::Yes);
            return;
        }
    }

    tcpSocket->abort();//取消已有链接
    QString hostip="127.0.0.1";
    int hosthost=8899;
    tcpSocket->connectToHost(hostip, hosthost);//链接服务器

    if(!tcpSocket->waitForConnected(30000))
    {
        QMessageBox::warning(this, "Warning!", "网络错误", QMessageBox::Yes);
    }
    else{
        QString message = QString("%1##change phone##%2").arg(LoginDialog::usrname).arg(phone_num);
        tcpSocket->write(message.toUtf8());
        tcpSocket->flush();
        connect(tcpSocket,&QTcpSocket::readyRead,[=](){
            QByteArray buffer = tcpSocket->readAll();
            if(QString(buffer).section("##",0,0)==QString("change phone success"))
            {
                QMessageBox::information(this, "Note", "更换手机号码成功！", QMessageBox::Yes);
                this->close();
            }
            else if(QString(buffer).section("##",0,0)==QString("change phone failed"))
            {
                qDebug() << "更换手机号码失败！";
                QMessageBox::warning(this, tr("警告"), tr("该手机号码已被其它用户使用！"), QMessageBox::Yes);
                ui->newPhone->text().clear();
                ui->newPhone->setFocus();
                return;

            }
        });
    }
}


void changePhone::on_returnBtn4_clicked()
{
    close();
    qDebug() << "退出修改手机号码界面";
}

