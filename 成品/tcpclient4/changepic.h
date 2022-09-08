#ifndef CHANGEPIC_H
#define CHANGEPIC_H

#include <QDialog>

namespace Ui {
class changePic;
}

class changePic : public QDialog
{
    Q_OBJECT

public:
    explicit changePic(QWidget *parent = nullptr);
    ~changePic();

private slots:
    void on_newPhotoBtn_clicked();
    void setEmptyPic();

    void on_doneBtn3_clicked();

    void on_returnBtn3_clicked();

private:
    Ui::changePic *ui;
};

#endif // CHANGEPIC_H
