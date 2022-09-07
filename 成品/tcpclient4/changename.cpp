#include "changename.h"
#include "ui_changename.h"
#include <QMessageBox>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QDebug>
#include "logindialog.h"

changeName::changeName(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::changeName)
{
    ui->setupUi(this);
    tcpSocket = new QTcpSocket();
}

changeName::~changeName()
{
    delete ui;
}

void changeName::on_doneBtn_clicked()
{
    bool containDigit = false;
    bool containAlpha = false;
    QString newName = ui->newName->text();
    int size = newName.size();

    //检查用户名格式
    if(newName==""){
        qDebug() << "更换用户名失败！";
        QMessageBox::warning(this, tr("警告"), tr("用户名不能为空！"), QMessageBox::Yes);
        return;
    }
    if(size<8 || size>20){
        qDebug() << "更换用户名失败！";
        QMessageBox::warning(this, tr("警告"), tr("用户名长度需介于8-20之间！"), QMessageBox::Yes);
        return;
    }
    if(newName.contains(" ")){
        qDebug() << "更换用户名失败！";
        QMessageBox::warning(this, tr("警告"), tr("该用户名无法使用！"), QMessageBox::Yes);
        return;
    }
    for(int i=0;i<size;i++){       //判断用户名是否包含字母和数字
        if(newName[i].isDigit()){
            containDigit = true;
        }
        else if(newName[i].isLetter()){
            containAlpha = true;
        }
    }
    if(!containAlpha || !containDigit){
        qDebug() << "更换用户名失败！";
        QMessageBox::warning(this, tr("警告"), tr("用户名需包含字母和数字！"), QMessageBox::Yes);
        return;
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
        QString message = QString("%1##change name##%2").arg(LoginDialog::usrname).arg(newName);
        tcpSocket->write(message.toUtf8());
        tcpSocket->flush();
        connect(tcpSocket,&QTcpSocket::readyRead,[=](){
            QByteArray buffer = tcpSocket->readAll();
            if(QString(buffer).section("##",0,0)==QString("change name success"))
            {
                LoginDialog::usrname = newName;
                QMessageBox::information(this, "Note", "更换用户名成功！", QMessageBox::Yes);
                this->close();
            }
            else if(QString(buffer).section("##",0,0)==QString("change name failed"))
            {
                qDebug() << "更换用户名失败！";
                QMessageBox::warning(this, tr("警告"), tr("该用户名已被其它用户使用！"), QMessageBox::Yes);
                ui->newName->text().clear();
                ui->newName->setFocus();
                return;

            }
        });

    }

}


void changeName::on_returnBtn_clicked()
{
    close();
    qDebug() << "退出修改用户名界面";
}

