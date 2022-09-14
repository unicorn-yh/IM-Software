#include "logindialog.h"
#include "ui_logindialog.h"
#include "registerdialog.h"
#include "mainwindow.h"
#include "QMessageBox"
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QDebug>
#include <QTcpSocket>
#include <QHostAddress>
#include <QDebug>
#include<QListWidgetItem>
#include"addfriend.h"
#include"delfriend.h"

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    tcpSocket = new QTcpSocket();
    this->setWindowTitle(tr("登录界面"));
    QString DIR="/media/sf_ShareUbuntu/";
    QPixmap mypix (DIR+"logo.png");
    ui->logo->setPixmap(mypix);
    ui->logo->setScaledContents(true);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}
QString LoginDialog::usrname="username";
QString LoginDialog::myport="8888";
void LoginDialog::on_LoginButton_clicked()
{

    QString username = ui->userLineEdit->text().trimmed();
    QString password = ui->pwdLineEdit->text();

    if(username == ""){
        QMessageBox::warning(this, "", "用户名不能为空");
    }
    else if(password == ""){
        QMessageBox::warning(this, "", "密码不能为空");
    }
    else{
        tcpSocket->abort();//取消已有链接
        QString hostip="127.0.0.1";
        int hosthost=8899;
        tcpSocket->connectToHost(hostip, hosthost);//链接服务器
        if(!tcpSocket->waitForConnected(30000))
        {
            QMessageBox::warning(this, "Warning!", "网络错误", QMessageBox::Yes);
        }
        else
        {
            qDebug()<<"success connect";
            QString loginmessage = QString("login##%1##%2").arg(username).arg(password);
            tcpSocket->write(loginmessage.toUtf8());
            tcpSocket->flush();
            connect(tcpSocket,&QTcpSocket::readyRead,[=](){
                QByteArray buffer = tcpSocket->readAll();
                if(QString(buffer).section("##",0,0)==QString("login successed"))
                {//登陆成功
                    QMessageBox::information(this, "Note", "登陆成功", QMessageBox::Yes);
                    LoginDialog::usrname=username;
                    LoginDialog::myport=QString(buffer).section("##",1,1);
                    qDebug()<<LoginDialog:: myport;
                    MainWindow *w=new MainWindow();
                    w->show();
                    this->close();
                }
                else if(QString(buffer).section("##",0,0)==QString("login error"))
                {
                    if(QString(buffer).section("##",1,1)==QString("no_user"))
                    {//用户不存在
                        QMessageBox::warning(this, "Warning!", "用户不存在", QMessageBox::Yes);

                    }
                    else if(QString(buffer).section("##",1,1)==QString("errpwd"))
                    {
                        QMessageBox::warning(this, "Warning!", "密码错误", QMessageBox::Yes);
                        ui->pwdLineEdit->clear();
                        ui->userLineEdit->setFocus();
                    }
                    else
                    {
                        QMessageBox::warning(this, "Warning!", "用户名或密码不为空", QMessageBox::Yes);
                        ui->userLineEdit->clear();
                        ui->pwdLineEdit->clear();
                        ui->userLineEdit->setFocus();
                    }
                }
            });
        }


    }

}


void LoginDialog::on_RegisterButton_clicked()
{
    RegisterDialog *regist = new RegisterDialog(this);
    regist->show();
    //this->close();
}

//void LoginDialog::on_RegisterButton_clicked()
//{
//    accept();
//}

