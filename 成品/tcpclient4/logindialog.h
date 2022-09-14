#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QTcpSocket>
#include <mainwindow.h>
namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();
    static QString usrname;
    static QString myport;
private slots:
    void on_LoginButton_clicked();
    void on_RegisterButton_clicked();

private:
    Ui::LoginDialog *ui;
    QTcpSocket *tcpSocket;
    MainWindow w;
};

#endif // LOGINDIALOG_H
