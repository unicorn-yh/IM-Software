#include "clientfile.h"
#include "ui_clientfile.h"
#include <QHostAddress>
#include <QMessageBox>

clientfile::clientfile(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::clientfile)
{
    ui->setupUi(this);
    ui->configBtn_2->hide();
    getClient(); //初始化和连接客户端
    setWindowTitle("文件传输");
}

clientfile::~clientfile()
{
    delete ui;
}
void clientfile::getClient()
{
    ui->lineEditIP->setText("127.0.0.1");
    ui->lineEditPort->setText(QString::number(port_number));
    isHead = true;
    socket2 = new QTcpSocket(this);
    ui->progress_bar->setValue(0); //初始化进度条值
    //ui->configBtn_2->setEnabled(false);

    connect(socket2, &QTcpSocket::connected,
    [=](){
        //提示连接成功
        ui->textEdit2->clear();
        ui->textEdit2->append("和服务器连接成功，等待服务器传送文件 ∙ ∙ ∙ ");
        }
    );

    connect(socket2, &QTcpSocket::readyRead,
    [=](){
        //取出接收的内容
        QByteArray buf = socket2->readAll();
        if(isHead){  //接收头
            isHead = false;

            //初始化文件信息
            fileName2 = QString(buf).section("##",0,0);  //文件名
            fileSize2 = QString(buf).section("##",1,1).toInt();  //文件大小
            recvSize = 0;   //已经接收文件大小

            //打开文件
            file2.setFileName(fileName2);  //关联文件名字
            bool validOpen = file2.open(QIODevice::WriteOnly);  //只写方式方式，打开文件
            if(!validOpen){
                qDebug() << "只写方式打开文件失败!";
                //ui->configBtn_2->setEnabled(false);
                QMessageBox::warning(this,tr("警告"),tr("只写方式打开文件失败!"),QMessageBox::Yes);
                socket2->disconnectFromHost(); //断开连接
                socket2->close();  //关闭套接字
                return;
            }

            //弹出对话框，显示接收文件的信息
            QString str = QString("接收的文件: [%1: %2kb]").arg(fileName2).arg(fileSize2/1024);
            //QMessageBox::information(this, "文件信息", str);
            ui->textEdit2->append(str);
            ui->textEdit2->append("正在接收文件 ∙ ∙ ∙ ");

            //设置进度条
            if(fileSize2!=0){
                ui->progress_bar->setMinimum(0); //最小值
                ui->progress_bar->setMaximum(fileSize2/1024); //最大值
                ui->progress_bar->setValue(0); //当前值
            }

        }
        else{ //文件信息
            qint64 len = file2.write(buf);
            if(len >0){ //接收数据大于0
                recvSize += len; //累计接收大小
                qDebug() << len << " byte";
            }
            ui->progress_bar->setValue(recvSize/1024);  //更新进度条
            if(recvSize == fileSize2){ //文件接收完毕
                socket2->write("文件接收完成！"); //发送文件完成接收信息给服务器
                ui->textEdit2->append("文件接收完成！");
                //ui->configBtn_2->setEnabled(false);
                ui->configBtn->setText("重新连接");
                QMessageBox::information(this, "完成", "文件接收完成！");
                file2.close();  //关闭文件
                socket2->disconnectFromHost();  //断开连接
                socket2->close();

            }
        }

        }
    );
}

void clientfile::on_configBtn_clicked()
{
    ui->configBtn->setText("连接");
    QString ip = ui->lineEditIP->text();  //服务器ip
    quint16 port = ui->lineEditPort->text().toInt();  //服务器端口号
    socket2->connectToHost(QHostAddress(ip), port); //连接服务器
    isHead = true;
    ui->progress_bar->setValue(0); //初始化进度条
}



/*void clientfile::on_configBtn_2_clicked()  //客户端取消文件传输
{
    ui->progress_bar->setValue(0);
   // ui->configBtn_2->setEnabled(false);
    ui->configBtn->setText("重新连接");
    qDebug() << "客户端取消文件接收！";
    socket2->write("客户端取消文件接收！");  //发送信息给服务器
    ui->textEdit2->append("取消文件接收！");
    file2.close();  //关闭文件
    socket2->disconnectFromHost();  //断开连接
    socket2->close();

}*/






