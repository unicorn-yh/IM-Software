#ifndef LOCALDB_H
#define LOCALDB_H

#include <QObject>

class LocalDB : public QObject
{
    Q_OBJECT

public:
    explicit LocalDB(QObject *parent = nullptr);

    QString getRecord(QString friendUserName);        //获取聊天记录
    void writeToDB(QString friendUserName, QString chatTime, QString message);       //把发进来的信息写入数据库中
    void createTable();



signals:

};

#endif // LOCALDB_H
