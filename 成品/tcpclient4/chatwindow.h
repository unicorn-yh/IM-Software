#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QMainWindow>
#include<QUdpSocket>
#include <QDialog>
#include<QDateTime>
#include<QListWidgetItem>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setitem(QListWidgetItem* u);

private slots:
    void send(int temport,QString ip);
    void recv();
    void on_sendmegbtn_clicked();
    void on_sendfilebtn_clicked();
    void connecttofriend();
    void on_connectbtn_clicked();

private:
    QListWidgetItem *b;
    Ui::MainWindow *ui;
    int port;
    int ip;
    //对方ip，对方端口，从数据库中获取
    QString ipStr;
    QUdpSocket *udpSocket;
    QDateTime current_date_time;
};
#endif // CHATWINDOW_H
