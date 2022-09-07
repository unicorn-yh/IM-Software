#include "changepassword.h"
#include "ui_changepassword.h"
#include <QMessageBox>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QDebug>
#include "logindialog.h"

changePassword::changePassword(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::changePassword)
{
    ui->setupUi(this);
    tcpSocket = new QTcpSocket();
}

changePassword::~changePassword()
{
    delete ui;
}


void changePassword::on_doneBtn2_clicked()
{
    bool validPassword = true;
    QString newPassword = ui->newPassword->text();
    QString newPassword2 = ui->newPassword2->text();
    int size = newPassword.size();
    int size2 = newPassword2.size();

    if(newPassword=="" || newPassword2==""){
        qDebug() << "更换用户密码失败！";
        QMessageBox::warning(this, tr("警告"), tr("密码不能为空！"), QMessageBox::Yes);
        ui->newPassword->clear();
        ui->newPassword2->clear();
        ui->newPassword->setFocus();
        return;
    }
    for(int i=0;i<size;i++){
        if(!(newPassword[i].isDigit())){
            qDebug() << "更换用户密码失败！";
            QMessageBox::warning(this, tr("警告"), tr("密码只能包含数字！"), QMessageBox::Yes);
            ui->newPassword->clear();
            ui->newPassword2->clear();
            ui->newPassword->setFocus();
            return;
        }
    }
    if(size<6 || size>12 || size2<6 || size2>12){
        qDebug() << "更换用户密码失败！";
        QMessageBox::warning(this, tr("警告"), tr("密码长度需介于6-12之间！"), QMessageBox::Yes);
        ui->newPassword->clear();
        ui->newPassword2->clear();
        ui->newPassword->setFocus();
        return;
    }
    if(newPassword != newPassword2){
        qDebug() << "更换用户密码失败！";
        QMessageBox::warning(this, tr("警告"), tr("输入密码不相同！"), QMessageBox::Yes);
        ui->newPassword->clear();
        ui->newPassword2->clear();
        ui->newPassword->setFocus();
        return;
    }


    if(validPassword == true){

        tcpSocket->abort();   //取消已有链接
        QString hostip="127.0.0.1";
        int hosthost=8899;
        tcpSocket->connectToHost(hostip, hosthost);//链接服务器

        if(!tcpSocket->waitForConnected(30000))
        {
            QMessageBox::warning(this, "Warning!", "网络错误", QMessageBox::Yes);
        }
        else{

            QString message = QString("%1##change password##%2").arg(LoginDialog::usrname).arg(newPassword);
            tcpSocket->write(message.toUtf8());
            tcpSocket->flush();
            connect(tcpSocket,&QTcpSocket::readyRead,[=](){
                QByteArray buffer = tcpSocket->readAll();
                if(QString(buffer).section("##",0,0)==QString("change password success"))
                {
                    QMessageBox::information(this, "Note", "更换用户密码成功！", QMessageBox::Yes);
                    this->close();
                }

            });

        }

    }
}


void changePassword::on_returnBtn2_clicked()
{
    close();
    qDebug() << "退出修改用户密码界面";
}

