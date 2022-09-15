#ifndef REGISTERDIALOG_H
#define REGISTERDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QTcpSocket>
namespace Ui {
class RegisterDialog;
}

class RegisterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RegisterDialog(QWidget *parent = nullptr);
    ~RegisterDialog();

private slots:

    void on_ExitButton_clicked();

    void on_RegisterButton_clicked();

private:
    Ui::RegisterDialog *ui;
    QTcpSocket *tcpSocket;
};

#endif // REGISTERDIALOG_H
