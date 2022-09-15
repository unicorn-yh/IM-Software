#include "settingdialog.h"
#include "ui_settingdialog.h"
#include "mylabel.h"
#include "changename.h"
#include "changepassword.h"
#include "changepic.h"
#include "changephone.h"
#include "blacklist.h"
#include <QMessageBox>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QHostAddress>
#include <QNetworkInterface>
#include "logindialog.h"

settingDialog::settingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::settingDialog)
{
    ui->setupUi(this);
    tcpSocket = new QTcpSocket();
    connect(ui->changeName,SIGNAL(Mouse_Pressed()),this,SLOT(change_Name()));
    connect(ui->changePassword,SIGNAL(Mouse_Pressed()),this,SLOT(change_Password()));
    connect(ui->changephoto,SIGNAL(Mouse_Pressed()),this,SLOT(change_Photo()));
    connect(ui->changephone,SIGNAL(Mouse_Pressed()),this,SLOT(change_Phone()));
    connect(ui->blacklist,SIGNAL(Mouse_Pressed()),this,SLOT(black_list()));
    connect(ui->logout,SIGNAL(Mouse_Pressed()),this,SLOT(log_out()));
}

settingDialog::~settingDialog()
{
    delete ui;
}

void settingDialog::change_Name()
{
   qDebug() << "进入修改用户名界面" ;
   changeName c;
   c.setWindowTitle("修改用户名");
   if(c.exec() == QDialog::Accepted){
       qDebug() << "更改用户名成功！";
       qDebug() << "退出修改用户名界面";
   }
}

void settingDialog::change_Password()
{
   qDebug() << "进入修改用户密码界面" ;
   changePassword c;
   c.setWindowTitle("修改用户密码");
   if(c.exec() == QDialog::Accepted){
       qDebug() << "更改用户密码成功！";
       qDebug() << "退出修改用户密码界面";
   }
}

void settingDialog::change_Photo()
{
   qDebug() << "进入修改个人头像界面" ;
   changePic c;
   c.setWindowTitle("修改个人头像");
   if(c.exec() == QDialog::Accepted){
       qDebug() << "更改个人头像成功！";
       qDebug() << "退出修改个人头像界面";
   }
}

void settingDialog::change_Phone()
{
   qDebug() << "进入修改手机号码界面" ;
   changePhone c;
   c.setWindowTitle("修改手机号码");
   if(c.exec() == QDialog::Accepted){
       qDebug() << "更改手机号码成功！";
       qDebug() << "退出修改手机号码界面";
   }
}
void settingDialog::black_list()
{
    qDebug() << "进入黑名单界面" ;
    blackList c;
    c.setWindowTitle("黑名单");
    if(c.exec() == QDialog::Accepted){
        qDebug() << "成功把好友拉出黑名单！";
        qDebug() << "退出黑名单界面";
    }
}

void settingDialog::log_out()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "提示", "是否退出登录？", QMessageBox::Yes|QMessageBox::No);
     if (reply == QMessageBox::Yes) {
         QString msg="用户退出登录";
         qDebug() << msg;

         tcpSocket->abort();//取消已有链接
         QString hostip="127.0.0.1";
         int hosthost=8899;
         tcpSocket->connectToHost(hostip, hosthost);//链接服务器

         if(!tcpSocket->waitForConnected(30000))
         {
             QMessageBox::warning(this, "Warning!", "网络错误", QMessageBox::Yes);
         }
         else{
             QString message = QString("%1##logout").arg(LoginDialog::usrname);
             tcpSocket->write(message.toUtf8());
             tcpSocket->flush();
             QApplication::quit();
         }

     }
}

