#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTcpServer>
#include <QTcpSocket>
#include <QLabel>
#include <sql.h>
#include <QTcpServer>
#include <QTcpSocket>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDateTime>
#include <QMessageBox>
#include <QFileDialog>
#include <QFileInfo>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{//start
    ui->setupUi(this);
    setWindowTitle("服务端");
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("userdata.db");
    if(!db.open()){
        qDebug() << "open DB error";
    }else{
        qDebug() << "Successfully open database";
    }

    QSqlQuery query;
    QString create_table = QString("create table if not exists userdata(username TEXT primary key , user_id integer auto_increment  , phone_number integer, sex TEXT, password TEXT,port integer unique)");
    QString create_table1=QString("create table if not exists friendlist (username text,friendname text,friendip text,friendport integer)");
    query.exec(create_table);
    query.finish();
    query.exec(create_table1);

    if(!query.exec()){
        qDebug() << "Error: fail to create table" << query.lastError();
    }else{
        qDebug() << "Table Created";
    }//reset database
      db.close();
    tcpServer = new QTcpServer();
    tcpServer->listen(QHostAddress::Any,8899);
    ui->lineEditIP_2->setText("127.0.0.1");
    ui->lineEditPort_2->setText(QString::number(8899));
    ui->fileBtn->setEnabled(false);  //设置上传文件按钮不能按
    ui->sendBtn->setEnabled(false);  //设置发送文件按钮不能按

    connect(tcpServer,&QTcpServer::newConnection,[=](){
           tcpSocket = tcpServer->nextPendingConnection();  //取出建立好连接的套接字
            QString ip = tcpSocket->peerAddress().toString(); //获取客户端的ip
            quint16 port_num = tcpSocket->peerPort();      //获取客户端的端口号
            ui->textEdit->setText(QString("[%1:%2] 成功连接.").arg(ip).arg(port_num)); //显示到编辑区
            ui->fileBtn->setEnabled(true); //成功连接后，上传文件按钮才能操作

            connect(tcpSocket,&QTcpSocket::readyRead,[=](){

                /*登录*/
               QByteArray buffer = tcpSocket->readAll();
               QString str=buffer;
               ui->record->append(str);
                if("login" == QString(buffer).section("##",0,0))
                {//登陆

                    db.setDatabaseName("./userdata.db");
                    db.open();
                    QSqlQuery sqlquery;
                    sqlquery.prepare("select * from userdata where username = :name");
                    sqlquery.bindValue(":name",QString(buffer).section("##",1,1));
                    sqlquery.exec();
                    if(!sqlquery.next())
                    {
                        tcpSocket->write(QString("login error##no_user").toUtf8());
                        tcpSocket->flush();
                        db.close();
                    }
                    else
                    {
                        QString pwd = sqlquery.value(4).toString();
                        if(pwd == QString(buffer).section("##",2,2))
                        {
                            sqlquery.prepare("select port from userdata where username = :name");
                            sqlquery.bindValue(":name",QString(buffer).section("##",1,1));
                            sqlquery.exec();
                            QString port;
                            if(sqlquery.next()){
                                    port=sqlquery.value(0).toString();
                            }
                            qDebug()<<port;
                            tcpSocket->write((QString("login successed##%1").arg(port)).toUtf8());
                            db.close();

                            }

                        else
                        {
                            tcpSocket->write(QString("login error##errpwd").toUtf8());
                            tcpSocket->flush();
                            db.close();
                        }

                }
           }
                else if("register" == QString(buffer).section("##",0,0))
                {
                    db.setDatabaseName("userdata.db");
                    db.open();
                    QSqlQuery sqlquery;
                    sqlquery.prepare("select * from userdata where username = :name");
                    sqlquery.bindValue(":name",QString(buffer).section("##",1,1));
                    sqlquery.exec();
                    if(!sqlquery.next())
                    {
                        sqlquery.clear();
                        sqlquery.prepare("insert into userdata (username,phone_number,sex,password,port) values (:name,:phone,:sex,:password,:port)");
                        sqlquery.bindValue(":name",QString(buffer).section("##",1,1));
                        sqlquery.bindValue(":password",QString(buffer).section("##",2,2));
                        sqlquery.bindValue(":phone",QString(buffer).section("##",3,3));
                        sqlquery.bindValue(":sex",QString(buffer).section("##",4,4));
                        sqlquery.bindValue(":port",QString(buffer).section("##",5,5));
                        sqlquery.exec();
                        tcpSocket->write(QString("register successed").toUtf8());
                        tcpSocket->flush();
                        db.close();
                    }
                    else
                    {
                         tcpSocket->write(QString("register error##same_name").toUtf8());
                         tcpSocket->flush();
                         db.close();
                    }
                }
                else if("addfriend"==QString(buffer).section("##",0,0)){
                    db.setDatabaseName("userdata.db");
                    db.open();
                    QSqlQuery sqlquery;
                    sqlquery.prepare("insert into friendlist (username,friendname,friendip,friendport) values(:name,:friendname,:friendip,:friendport)");
                    sqlquery.bindValue(":name",QString(buffer).section("##",1,1));
                    sqlquery.bindValue(":friendname",QString(buffer).section("##",2,2));
                    sqlquery.bindValue(":friendip",QString(buffer).section("##",3,3));
                    sqlquery.bindValue(":friendport",QString(buffer).section("##",4,4));
                    sqlquery.exec();
                    tcpSocket->write(QString("successed").toUtf8());
                    tcpSocket->flush();
                    db.close();
                }


                /*文件传输*/
                if(QString(buffer) == "file done"){ //文件接收完毕
                     ui->textEdit->append("文件发送完毕!");
                     file.close();
                     tcpSocket->disconnectFromHost();  //断开客户端端口
                     tcpSocket->close();
                }
                if("checkfriendlist"==QString(buffer).section("##",0,0)){

                    db.setDatabaseName("userdata.db");
                    db.open();
                    QSqlQuery sqlquery;
                    sqlquery.prepare("select friendname from friendlist where username = :name");
                    sqlquery.bindValue(":name",QString(buffer).section("##",1,1));
                    sqlquery.exec();
                    QString section="";
                    while(sqlquery.next())
                    {
                        section=section+sqlquery.value(0).toString()+'#';

                    }
                    qDebug()<<section;
                    tcpSocket->write(section.toUtf8());
                    tcpSocket->flush();
                    db.close();
                }
                if("delfriend"==QString(buffer).section("##",0,0)){

                    db.setDatabaseName("userdata.db");
                    db.open();
                    QSqlQuery sqlquery;
                    sqlquery.prepare("delete from friendlist where username = :name and friendname =:fname");
                    sqlquery.bindValue(":name",QString(buffer).section("##",1,1));
                    sqlquery.bindValue(":fname",QString(buffer).section("##",2,2));
                    sqlquery.exec();

                    db.close();
                }

                if("checkuserdata"==QString(buffer).section("##",0,0)){

                                    db.setDatabaseName("userdata.db");
                                    db.open();
                                    QSqlQuery sqlquery;
                                    sqlquery.prepare("select port from userdata where username = :name");
                                    sqlquery.bindValue(":name",QString(buffer).section("##",1,1));
                                    sqlquery.exec();
                                    QString port;
                                    if(sqlquery.next()){
                                            port=sqlquery.value(0).toString();
                                            qDebug()<<"sql respond = "+port;
                                            tcpSocket->write(port.toUtf8());
                                            tcpSocket->flush();
                                    }

                                    db.close();
                }


                /*设置*/

                /*更新用户名*/
                if("change name" == QString(buffer).section("##",1,1))
                {
                    db.setDatabaseName("userdata.db");
                    db.open();
                    QSqlQuery sqlquery;
                    sqlquery.prepare("select * from userdata where username = :name");
                    sqlquery.bindValue(":name",QString(buffer).section("##",2,2));
                    sqlquery.exec();
                    if(!sqlquery.next())   //检查用户名是否被其他用户使用过
                    {
                        sqlquery.clear();
                        sqlquery.prepare("UPDATE userdata SET username=:name where username=:oldname");  //更新数据库
                        sqlquery.bindValue(":oldname",QString(buffer).section("##",0,0));
                        sqlquery.bindValue(":name",QString(buffer).section("##",2,2));
                        sqlquery.exec();
                        tcpSocket->write(QString("change name success").toUtf8());
                        tcpSocket->flush();
                        db.close();
                    }
                    else
                    {
                         tcpSocket->write(QString("change name failed##same_name").toUtf8());
                         tcpSocket->flush();
                         db.close();
                    }

                }

                /*更新用户密码*/
                if("change password" == QString(buffer).section("##",1,1))
                {
                    db.setDatabaseName("userdata.db");
                    db.open();
                    QSqlQuery sqlquery;
                    sqlquery.prepare("UPDATE userdata SET password=:pwd where username=:name");  //更新数据库
                    sqlquery.bindValue(":name",QString(buffer).section("##",0,0));
                    sqlquery.bindValue(":pwd",QString(buffer).section("##",2,2));
                    sqlquery.exec();
                    tcpSocket->write(QString("change password success").toUtf8());
                    tcpSocket->flush();
                    db.close();
                }

                /*更新手机号码*/
                if("change phone" == QString(buffer).section("##",1,1))
                {
                    db.setDatabaseName("userdata.db");
                    db.open();
                    QSqlQuery sqlquery;
                    sqlquery.prepare("select * from userdata where phone_number = :phone");
                    sqlquery.bindValue(":phone",QString(buffer).section("##",2,2));
                    sqlquery.exec();
                    if(!sqlquery.next())   //检查用户名是否被其他用户使用过
                    {
                        sqlquery.clear();
                        sqlquery.prepare("UPDATE userdata SET phone_number=:phone where username=:name");  //更新数据库
                        sqlquery.bindValue(":name",QString(buffer).section("##",0,0));
                        sqlquery.bindValue(":phone",QString(buffer).section("##",2,2));
                        sqlquery.exec();
                        tcpSocket->write(QString("change phone success").toUtf8());
                        tcpSocket->flush();
                        db.close();
                    }
                    else
                    {
                         tcpSocket->write(QString("change phone failed##same_phone").toUtf8());
                         tcpSocket->flush();
                         db.close();
                    }

                }

            });

    });

    connect(&timer, &QTimer::timeout,
        [=]()
        {
             timer.stop();  //关闭定时器
             sendFileData();  //发送文件数据
        }

     );
}

MainWindow::~MainWindow()
{
    delete ui;
}

//选择文件的按钮
void MainWindow::on_fileBtn_clicked()
{
    QString fileDir = QFileDialog::getOpenFileName(this,"open","");
    if(!fileDir.isEmpty()){ //如果选择文件路径有效
        fileName.clear();
        fileSize = 0;

        //获取文件信息
        QFileInfo info(fileDir);
        fileName = info.fileName(); //获取文件名字
        fileSize = info.size(); //获取文件大小
        sendSize = 0; //发送文件的大小

        //只读方式打开文件
        file.setFileName(fileDir);
        bool validOpen = file.open(QIODevice::ReadOnly); //打开文件
        if(!validOpen){
            qDebug() << "只读方式打开文件失败!";
            QMessageBox::warning(this,tr("警告"),tr("只读方式打开文件失败!"),QMessageBox::Yes);

        }

        //提示打开文件的路径
        ui->textEdit->append(fileDir);   //服务器上更新文件路径
        ui->fileBtn->setEnabled(false);  //设置上传文件按钮可以按
        ui->sendBtn->setEnabled(true);   //设置发送文件按钮可以按

    }
    else{
        qDebug() << "选择文件路径出错!";
        QMessageBox::warning(this,tr("警告"),tr("选择文件路径出错!"),QMessageBox::Yes);
    }

}

//发送文件按钮
void MainWindow::on_sendBtn_clicked()
{

    ui->lineEditPort_2->setFocus();
    ui->lineEditPort_2->clearFocus();
    ui->sendBtn->setEnabled(false);
    QString head = QString("%1##%2").arg(fileName).arg(fileSize);  //先发送文件头信息  文件名##文件大小
    qint64 len = tcpSocket->write(head.toUtf8());  //发送头部信息
    if(len > 0){ //头部信息发送成功
        timer.start(20); //发送文件信息时，为了防止TCP黏包，需使用定时器延时20ms
    }
    else{
        qDebug() << "头部信息发送失败！";
        QMessageBox::warning(this,tr("警告"),tr("头部信息发送失败！"),QMessageBox::Yes);
        file.close();
        ui->fileBtn->setEnabled(true);
        ui->sendBtn->setEnabled(false);
    }
}

void MainWindow::sendFileData()
{
    ui->textEdit->append("正在发送文件 ∙ ∙ ∙ ");
     qint64 len = 0;
     do{
        char buffer[4*1024] = {0}; //数据大小
        len = 0;
        len = file.read(buffer, sizeof(buffer)); //读数据
        len = tcpSocket->write(buffer, len); //发数据
        sendSize += len; //发送数据的总大小
     }while(len > 0);

     if(sendSize==fileSize){
         ui->textEdit->append("文件发送完毕！");
     }

}

