#include "WidgetPreview.h"
#include "ui_WidgetPreview.h"

#include <QDebug>

WidgetPreview::WidgetPreview(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetPreview)
{
    ui->setupUi(this);

    _mouseStartPosX=0;
    _mouseStartPosY=0;

    ui->label_2->setPixmap(QPixmap(QString(PATH_USERDATA)+"/images/buttons/close.png"));
    ui->label_2->installEventFilter(this);

    ui->label->installEventFilter(this);
    ui->label->setScaledContents(true);
    ui->label->setPixmap(QPixmap());

}

WidgetPreview::~WidgetPreview(){
    delete ui;
}

void WidgetPreview::setPicture(QPixmap picture){
    ui->label->setPixmap(picture);

    double sizeDefaultWidth = 321;
    double sizeDefaultHeight = 181;

    QSize sizePicture = picture.size();
    this->resize(sizePicture);
    double deltaW = (double)this->width()/sizeDefaultWidth;
    this->resize(sizeDefaultWidth,(double)this->height()/deltaW);

    if (this->height()>181){
        double deltaH = (double)this->height()/sizeDefaultHeight;
        this->resize((double)this->width()/deltaH,sizeDefaultHeight);
    }


    ui->label->setGeometry(0,0,this->width(),this->height());
    ui->label_2->move(this->width()-20,2);

}

bool WidgetPreview::eventFilter(QObject *obj, QEvent *event){
    QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);

    if (event->type() == QEvent::MouseButtonRelease){
        if (obj==ui->label_2){
            this->hide();
            return true;
        }


    }
    if (event->type() == QEvent::MouseButtonPress){
        if (obj==ui->label){
            _mouseStartPosX=mouseEvent->globalX();
            _mouseStartPosY=mouseEvent->globalY();
            return true;
        }
    }

    if (event->type() == QEvent::MouseMove){
        if (obj==ui->label){
            this->move((this->x())+(mouseEvent->globalX()-_mouseStartPosX),(this->y())+(mouseEvent->globalY()-_mouseStartPosY));
            _mouseStartPosX=mouseEvent->globalX();
            _mouseStartPosY=mouseEvent->globalY();
        }
    }

    return QObject::eventFilter(obj, event);
}

void WidgetPreview::mousePressEvent(QMouseEvent *event){

}

void WidgetPreview::mouseMoveEvent(QMouseEvent *event){

}

