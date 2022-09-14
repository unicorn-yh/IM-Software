#include "logindialog.h"
#include <QApplication>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LoginDialog dlg;
    dlg.show();
    /*MainWindow w;
    w.show();*/
    return a.exec();
}
