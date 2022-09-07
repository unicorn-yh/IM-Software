#ifndef BLACKLIST_H
#define BLACKLIST_H

#include <QDialog>

namespace Ui {
class blackList;
}

class blackList : public QDialog
{
    Q_OBJECT

public:
    explicit blackList(QWidget *parent = nullptr);
    ~blackList();

private:
    Ui::blackList *ui;

private slots:
    void displayBlackListUser();
    void outBtn();
};

#endif // BLACKLIST_H
