#include "localdb.h"

#include <QSqlDatabase>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QDateTime>
LocalDB::LocalDB(QObject *parent) : QObject(parent)
{

}

QString LocalDB::getRecord(QString friendUserName)
{
    QSqlDatabase db;
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("chat_record.db");
    if(!db.open()){
        qDebug() << "open DB error";
    }else{
        qDebug() << "Successfully open database";
    }

    QSqlQuery query;
    query.exec(QString("select * from chat_record where username='%1'").arg(friendUserName));

    for(int i = 0; i < 10; i++){
        qDebug() << query.value(0).toString();
        query.next();
    }
    //TODO
    return "这是一条聊天记录";
}

void LocalDB::writeToDB(QString speaker, QString listener, QString message)
{
    QSqlDatabase db;
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("chat_record.db");
    if(!db.open()){
        qDebug() << "open DB error";
    }else{
        qDebug() << "Successfully open database";
    }
    QDateTime datetime=QDateTime::currentDateTime();
    QString time=datetime.toString("yyyy-MM-dd hh:mm:ss");
    QSqlQuery query;
    QString qs = QString("insert into chat_record(speakername, listenername,time, chat_text) values (%1, %2, %3, %4 )")
            .arg(speaker,listener, time, message);
    query.exec(qs);

    return;
}

void LocalDB::createTable()
{
    QSqlDatabase db;
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("chat_record.db");
    if(!db.open()){
        qDebug() << "open DB error";
    }else{
        qDebug() << "Successfully open database";
    }

    QSqlQuery query(db);
    QString create_table =
            QString("create table if not exists \
                     chat_record(speakername text,listenername text, time text primary key, chat_text text)");

    query.prepare(create_table);

    if(!query.exec()){
        qDebug() << "Error: fail to create table" << query.lastError();
    }else{
        qDebug() << "Table Created";
    }
}
