#ifndef CHANGEPASSWORD_H
#define CHANGEPASSWORD_H

#include <QDialog>
#include <QTcpSocket>

namespace Ui {
class changePassword;
}

class changePassword : public QDialog
{
    Q_OBJECT

public:
    explicit changePassword(QWidget *parent = nullptr);
    ~changePassword();

private slots:

    void on_doneBtn2_clicked();
    void on_returnBtn2_clicked();

private:
    Ui::changePassword *ui;
    QTcpSocket *tcpSocket;
};

#endif // CHANGEPASSWORD_H
