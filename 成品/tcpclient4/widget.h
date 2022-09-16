#ifndef WIDGET_H
#define WIDGET_H

#include<QWidget>
#include<QUdpSocket>
#include<QColorDialog>
namespace Ui {
    class Widget;
}

//枚举变量标志信息类型，分别有消息、新用户加入、用户退出
enum MessageType{
    Message, NewParticipant,ParticipantLeft
};

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QString niName = "一颗野指针" , QWidget *parent = 0);
    ~Widget();

protected:
    void newParticipant(QString userName,
                        QString localHostName,
                        QString ipAdress);
    void participantLeft(QString userName,
                         QString localHostName,
                         QString time);
    void sendMessage(MessageType type, QString serverAdress="");

    QString getIP();
    QString getUserName();
    QString getMessage();

    void setDefaultName(QString name);


private:
    Ui::Widget *ui;
    QUdpSocket *udpSocket;
    qint16 port;
    QColor color;
    QString defaultName = "一颗野指针";

private slots:
    void processPendinDatagrames();
    void on_sendButton_clicked();
    void on_exitButton_clicked();
    void on_fontComboBox_currentFontChanged(const QFont &f);
    void on_italicToolBtn_clicked(bool checked);
    void on_sizeComboBox_activated(const QString &arg1);
    void on_boldToolBtn_clicked(bool checked);
    void on_colorToolBtn_clicked();
};

#endif // WIDGET_H
