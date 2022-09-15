#ifndef MYLABEL_H
#define MYLABEL_H

#include <QLabel>
#include <QMouseEvent>
#include <QDebug>
#include <QEvent>

class myLabel : public QLabel
{
    Q_OBJECT
public:
    myLabel(QWidget * parent = 0 );
    ~myLabel(){}
    void mousePressEvent (QMouseEvent *event);

signals:
    void Mouse_Pressed();

public slots:
    void slotClicked();


};
#endif // MYLABEL_H
