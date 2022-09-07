#ifndef CHANGENAME_H
#define CHANGENAME_H

#include <QDialog>
#include <QTcpSocket>

namespace Ui {
class changeName;
}

class changeName : public QDialog
{
    Q_OBJECT

public:
    explicit changeName(QWidget *parent = nullptr);
    ~changeName();

private slots:
    void on_doneBtn_clicked();
    void on_returnBtn_clicked();

private:
    Ui::changeName *ui;
    QTcpSocket *tcpSocket;
};

#endif // CHANGENAME_H
