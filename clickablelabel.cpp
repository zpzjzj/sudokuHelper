#include "clickablelabel.h"
#include <QMouseEvent>
#include <QDebug>

ClickableLabel::ClickableLabel(QWidget *parent) :
    QLabel(parent)
{
}
void ClickableLabel::mousePressEvent ( QMouseEvent * event )
{
    qDebug( ) << "click " << event->button();
    if (event->button() == Qt::RightButton)
        emit rclicked();
    else if(event->button() == Qt::LeftButton)
        emit clicked();
}
