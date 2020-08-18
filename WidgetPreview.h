#ifndef WIDGETPREVIEW_H
#define WIDGETPREVIEW_H

#include <QWidget>
#include <QMouseEvent>

namespace Ui {
class WidgetPreview;
}

class WidgetPreview : public QWidget
{
    Q_OBJECT

public:
        explicit WidgetPreview(QWidget *parent = nullptr);
        ~WidgetPreview();

        void setPicture(QPixmap picture);

    protected:
        bool eventFilter(QObject *obj, QEvent *event) override;
        void mousePressEvent(QMouseEvent *event) override;
        void mouseMoveEvent(QMouseEvent *event) override;



    private:
        Ui::WidgetPreview *ui;

        int _mouseStartPosX;
        int _mouseStartPosY;
};

#endif // WIDGETPREVIEW_H
