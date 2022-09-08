#include "chatwindow.h"
#include "ui_mainwindow.h"
#include<QMessageBox>
#include<QtDebug>

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    udpSocket=new QUdpSocket(this);
    port='8888';
    ip='127.0.0.1';
    ui->sendmegbtn->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::connecttofriend(){
    bool bindResult = udpSocket->bind(QHostAddress::AnyIPv4, port);
    if(!bindResult)
       {
          QMessageBox::warning(this, tr("Waring"),
                                    tr("binding error!"),
                                       QMessageBox::Yes);
             return;
          }else{
          QMessageBox::warning(NULL,tr("note"),tr("binging success!"));
     }
     ui->sendmegbtn->setEnabled(true);
     connect(ui->sendmegbtn, SIGNAL(clicked()), this, SLOT(send()));
     connect(udpSocket, SIGNAL(readyRead()), this, SLOT(recv()));
}

void MainWindow::send(int temport,QString ipStr){
    QString sendStr =ui->msgEdit->text();
    qDebug() << "sendData str" << sendStr;
    if(sendStr.isEmpty())
    {
        QMessageBox::warning(this, tr("Waring"),
                              tr("Text is empty!"),
                                 QMessageBox::Yes);
        return;
    }
    int length = udpSocket->writeDatagram(
                sendStr.toStdString().c_str(),
                QHostAddress(ipStr.toStdString().c_str()),temport);
    current_date_time =QDateTime::currentDateTime();
    QString current_date =current_date_time.toString("yyyy.MM.dd hh:mm:ss.zzz ddd");
    ui->textEdit->insertPlainText(current_date+"\n");
    ui->textEdit->insertPlainText("Me:"+sendStr + "\n");
}

void MainWindow::recv(){
    while(udpSocket->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(datagram.data(), datagram.size());
        QString msg = datagram.data();
        ui->textEdit->insertPlainText("Peer:"+msg + "\n");
    }
}


void MainWindow::on_sendmegbtn_clicked()
{
    QString name=ui->nameEdit->text();
    //ip,port=getdata(name);
    //send(ip,port);
}

void MainWindow::setitem(QListWidgetItem *u){
    b=u;
    ui->nameEdit->setText(b->text());
}

void MainWindow::on_sendfilebtn_clicked()
{

}



void MainWindow::on_connectbtn_clicked()
{
    connecttofriend();
}

