#ifndef CLIENTFILE_H
#define CLIENTFILE_H
#include <QTcpSocket>
#include <QFile>
#include <QDialog>

namespace Ui {
class clientfile;
}

class clientfile : public QDialog
{
    Q_OBJECT

public:
    explicit clientfile(QWidget *parent = nullptr);
    ~clientfile();


private slots:
    void on_configBtn_clicked();  //客户端的连接按钮
   // void on_configBtn_2_clicked(); //客户端的取消文件传输按钮
    void getClient();  //初始化和连接客户端

private:
    Ui::clientfile *ui;
    int port_number = 8899;

    //客户端私有变量
    QTcpSocket *socket2;
    QFile file2;       //接受的文件对象
    QString fileName2; //接受文件名
    qint64 fileSize2; //文件大小
    qint64 recvSize; //已经接收文件的大小
    bool isHead;   //标志位，是否为头部信息


};

#endif // CLIENTFILE_H
