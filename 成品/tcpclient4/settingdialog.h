#ifndef SETTINGDIALOG_H
#define SETTINGDIALOG_H

#include <QDialog>
#include <QTcpSocket>

namespace Ui {
class settingDialog;
}

class settingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit settingDialog(QWidget *parent = nullptr);
    ~settingDialog();

private slots:
    void change_Name();
    void change_Password();
    void change_Photo();
    void change_Phone();
    void black_list();
    void log_out();

private:
    Ui::settingDialog *ui;
    QTcpSocket *tcpSocket;
};

#endif // SETTINGDIALOG_H
