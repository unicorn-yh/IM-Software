#include "mylabel.h"

myLabel::myLabel(QWidget * parent):QLabel(parent)
{
    //connect( this, SIGNAL( clicked() ), this, SLOT( slotClicked() ) );
}

void myLabel::slotClicked()
{
    qDebug()<<"Clicked";
}

void myLabel::mousePressEvent (QMouseEvent * event)
{
    emit Mouse_Pressed();
}
