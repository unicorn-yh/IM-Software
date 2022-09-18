#include "sql.h"
#include <QString>
Sql::Sql()
{

}
int Sql::sqllogin(QSqlDatabase db,char username,char password)
{
    QSqlQuery query(db);
    QString qs = QString("select * from user where username = '%1' and password = '%2' ").arg(username, password);
    if(!query.exec(qs)){
        qDebug() << query.lastError().text();
        return 1;//unknown error
    }
    if(query.next())
    {
        return 2;//successfully login
    } else {
       return 3;//username or password error
    }
}

int Sql::sqlregister(QSqlDatabase db,char username ,int phone_number , char sex , char password)
{
    QSqlQuery query(db);
    QString qStr = QString("insert into %1  (username, password, phone_number, sex) values ('%2', '%3', '%4', '%5')").arg("user", username, password, phone_number, sex);
    qDebug() << qStr;
    query.exec(qStr);
    return 1;//success
}
