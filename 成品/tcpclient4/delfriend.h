#ifndef DELFRIEND_H
#define DELFRIEND_H

#include <QDialog>
#include <QTcpSocket>
#include <QMessageBox>
#include <logindialog.h>
namespace Ui {
class delfriend;
}

class delfriend : public QDialog
{
    Q_OBJECT

public:
    explicit delfriend(QWidget *parent = nullptr);
    ~delfriend();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::delfriend *ui;
    QTcpSocket *tcpSocket;
};

#endif // DELFRIEND_H
