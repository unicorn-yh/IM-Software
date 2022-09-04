#ifndef ADDFRIEND_H
#define ADDFRIEND_H

#include <QDialog>
#include <QTcpSocket>
#include <logindialog.h>
namespace Ui {
class addfriend;
}

class addfriend : public QDialog
{
    Q_OBJECT

public:
    explicit addfriend(QWidget *parent = nullptr);
    ~addfriend();

private slots:
    void on_sure_clicked();

private:
    Ui::addfriend *ui;
    QTcpSocket *tcpSocket;
};

#endif // ADDFRIEND_H
