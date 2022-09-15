#include "registerdialog.h"
#include "ui_registerdialog.h"
#include "QMessageBox"
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QDebug>
#include "logindialog.h"

RegisterDialog::RegisterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegisterDialog)
{
    ui->setupUi(this);
    tcpSocket = new QTcpSocket();
    this->setWindowTitle(tr("注册界面"));
}

RegisterDialog::~RegisterDialog()
{
    delete ui;
}

void RegisterDialog::on_ExitButton_clicked()
{
    this->close();
}

void RegisterDialog::on_RegisterButton_clicked()
{
    QString username = ui->userLineEdit->text().trimmed();
    QString password = ui->pwdLineEdit->text();
    QString password2 = ui->pwd2LineEdit->text();
    QString phone_number = ui->phoneLineEdit->text();
    QString sex = ui->sexLineEdit->text();
    QString port=ui->portLineEdit->text();
    int size = username.size();
    int gender = sex.toInt();
    if(username == ""){
        QMessageBox::warning(this, tr("警告"), tr("用户名不能为空"), QMessageBox::Yes);
    }
    else{
            if(sex != "0" && sex != "1"){
                QMessageBox::warning(this, tr("警告"), tr("请输入正确的性别"), QMessageBox::Yes);
                ui->sexLineEdit->clear();
            }
            else{
                if(password != password2){
                    QMessageBox::warning(this, tr("警告"), tr("两次密码不一致"), QMessageBox::Yes);
                    ui->userLineEdit->clear();
                    ui->pwdLineEdit->clear();
                    ui->pwd2LineEdit->clear();

                }else{
                    tcpSocket->abort();
                    tcpSocket->connectToHost("127.0.0.1", 8899);
                    if(!tcpSocket->waitForConnected(30000))
                    {
                        QMessageBox::warning(this, "Warning!", "网络错误", QMessageBox::Yes);
                    }
                    else {
                                if(size < 8 || size > 20){
                                    QMessageBox::warning(this, tr("警告"), tr("用户名的长度应在8-20之间"), QMessageBox::Yes);
                                    ui->userLineEdit->clear();
                                    ui->userLineEdit->setFocus();
                                }
                                else{
                                    bool nameContainDigit = false;
                                    bool nameContainAlpha = false;

                                    for(int i=0;i<size;i++){       //判断用户名是否包含字母和数字
                                        if(username[i].isDigit()){
                                            nameContainDigit = true;
                                        }
                                        else if(username[i].isLetter()){
                                            nameContainAlpha = true;
                                        }
                                    }

                                    if(!nameContainAlpha || !nameContainDigit){
                                        qDebug() << "the username is invalid";
                                        QMessageBox::warning(this, tr("警告"), tr("用户名需包含字母和数字！"), QMessageBox::Yes);
                                        ui->userLineEdit->clear();
                                        ui->userLineEdit->setFocus();
                                    }
                                    else
                                    {
                                        if(port.toInt()<8000||port.toInt()>10000){
                                            QMessageBox::warning(this, tr("警告"), tr("端口号需小于10000,大于8000！"), QMessageBox::Yes);
                                            ui->portLineEdit->clear();
                                            ui->portLineEdit->setFocus();
                                        }else{
                                            QString xingbie;
                                            switch(gender){
                                            case 1:
                                                xingbie = "男";
                                                break;
                                            case 0:
                                                xingbie = "女";
                                                break;

                                            default:
                                                break;
                                            }
                                            QString loginmessage = QString("register##%1##%2##%3##%4##%5").arg(username).arg(password).arg(phone_number).arg(xingbie).arg(port);
                                                tcpSocket->write(loginmessage.toUtf8());
                                                tcpSocket->flush();
                                                connect(tcpSocket,&QTcpSocket::readyRead,[=](){
                                                    QByteArray buffer = tcpSocket->readAll();
                                                    if(QString(buffer).section("##",0,0)==QString("register successed"))
                                                    {
                                                        QMessageBox::information(this, tr("Note"), tr("注册成功"),
                                                                                   QMessageBox::Yes);
                                                        this->close();
                                                    }
                                                    else if(QString(buffer).section("##",0,0)==QString("register error"))
                                                    {
                                                        if(QString(buffer).section("##",1,1)==QString("same_name"))
                                                        {
                                                            QMessageBox::warning(this, "Warning!", "昵称有重复", QMessageBox::Yes);
                                                            ui->userLineEdit->clear();
                                                            ui->userLineEdit->setFocus();
                                                        }
                                                    }
                                                });
                                         accept();
                                        }


                                    }


                       }





                }
            }
        }
}
}

