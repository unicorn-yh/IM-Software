#ifndef CHANGEPHONE_H
#define CHANGEPHONE_H

#include <QDialog>
#include <QTcpSocket>

namespace Ui {
class changePhone;
}

class changePhone : public QDialog
{
    Q_OBJECT

public:
    explicit changePhone(QWidget *parent = nullptr);
    ~changePhone();

private slots:
    void on_doneBtn4_clicked();
    void on_returnBtn4_clicked();

private:
    Ui::changePhone *ui;
    QTcpSocket *tcpSocket;
};

#endif // CHANGEPHONE_H
