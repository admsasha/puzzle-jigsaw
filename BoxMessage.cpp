#include "BoxMessage.h"

#include <QBrush>
#include <QPalette>
#include <QMouseEvent>

#include "config_puzzle.h"

BoxMessage::BoxMessage(QWidget *parent) : QLabel(parent){
    this->setPixmap(QPixmap(QString(PATH_USERDATA)+"/images/box_message.png"));
    this->setScaledContents(true);

    isMove=false;

    QBrush brush;
    QPalette palette;

    brush.setColor(QColor(255, 255, 255, 255));
    brush.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
    palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);


    lblText = new QLabel(this);
    lblText->setFont((QFont("",36)));
    lblText->setPalette(palette);
    lblText->setAlignment(Qt::AlignCenter);
    lblText->installEventFilter(this);

}

BoxMessage::~BoxMessage(){
    delete lblText;
}

void BoxMessage::resizeEvent(QResizeEvent */*e*/){
    lblText->setGeometry(0,0,this->width(),this->height());
}

bool BoxMessage::eventFilter(QObject *obj, QEvent *event){
    QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);


    if (event->type() == QEvent::MouseButtonPress){
        if (obj==lblText){
            mousePosition=QPointF(mouseEvent->globalX(),mouseEvent->globalY());
            isMove=true;
            return true;
        }
    }
    if (event->type() == QEvent::MouseButtonRelease){
        isMove=false;
    }

    if (event->type() == QEvent::MouseMove){
        if (isMove==true){
            this->move(this->x()+ (mouseEvent->globalX()-mousePosition.x()),this->y()+ (mouseEvent->globalY()-mousePosition.y()));
            mousePosition=QPointF(mouseEvent->globalX(),mouseEvent->globalY());
        }
    }

    return QObject::eventFilter(obj, event);
}


void BoxMessage::setText(QString text){
    lblText->setText(text);
}
