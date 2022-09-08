#include "changepic.h"
#include "ui_changepic.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QDebug>
#include <QBuffer>


changePic::changePic(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::changePic)
{
    ui->setupUi(this);
    setEmptyPic();
}

changePic::~changePic()
{
    delete ui;
}

void changePic::setEmptyPic(){

    //打开数据库
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("im.db");
    if(!db.open()){
        qDebug() << "open database error";
        return;
    }
    QSqlQuery query;

    //select query
    /*int user_id = 1;
    query.prepare("SELECT photo FROM user WHERE id = :myid");
    query.bindValue(":myid", user_id);
    query.exec();
    //query.exec("select photo from user ");

    //get photo
    while(query.next()){
        //qDebug() << query.value(0).toByteArray();
        QByteArray pData = query.value(0).toByteArray();
        if(pData.size()){
             QPixmap p;
             if(p.loadFromData(pData)){  // fill array with image
                ui->photo->setPixmap(p);
                ui->photo->setScaledContents(true);
             }
             else{
                 qDebug() << "error";
             }
        }
        else{
            QPixmap mypix (DIR+"user.png");
            ui->photo->setPixmap(mypix);
            ui->photo->setScaledContents(true);
        }
    }*/

    QString DIR="/media/sf_ShareUbuntu/";
    QPixmap mypix (DIR+"upload.png");
    ui->photo->setPixmap(mypix);
    ui->photo->setScaledContents(true);

}

void changePic::on_newPhotoBtn_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,tr("选择图片"),"",tr("Images(*.png *.jpg *.jpeg *.bmp *.gif)"));
    if(QString::compare(filename,QString())!=0){
        qDebug() << filename;
        QImage img;
        bool validImage = img.load(filename);
        if(validImage){
            ui->photo->setPixmap(QPixmap::fromImage((img)));
            ui->photo->setScaledContents(true);
        }
        else{
            QMessageBox::warning(this, tr("警告"), tr("上传图片失败！"), QMessageBox::Yes);
        }
    }
}


void changePic::on_doneBtn3_clicked()
{
    //打开数据库
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("im.db");
    if(!db.open()){
        qDebug() << "open database error";
        return;
    }
    QSqlQuery query;

    //update query
    const QPixmap* pixmap = ui->photo->pixmap();
    if(pixmap){
        int user_id = 1;
        QByteArray bytes;
        QBuffer buffer(&bytes);
        buffer.open(QIODevice::WriteOnly);
        pixmap->save(&buffer, "PNG");
        query.prepare("UPDATE user SET photo=:photo2 WHERE id=:id2");  //更新数据库的用户头像
        query.bindValue(":photo2", pixmap);
        query.bindValue(":id2", user_id);
        query.exec();
        db.close();
    }

    QMessageBox::information(NULL, tr("提示"), tr("成功更换新个人头像！"));
    accept();
}


void changePic::on_returnBtn3_clicked()
{
    close();
    qDebug() << "退出修改个人头像界面";
}

