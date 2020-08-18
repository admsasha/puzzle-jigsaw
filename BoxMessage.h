#ifndef BOXMESSAGE_H
#define BOXMESSAGE_H

#include <QLabel>
#include <QEvent>

class BoxMessage : public QLabel {
    public:
        BoxMessage(QWidget *parent = nullptr);
        ~BoxMessage();

        void setText(QString text);

    private:
        QLabel *lblText;

        QPointF mousePosition;
        bool isMove;

    protected:
        void resizeEvent(QResizeEvent */*e*/) override;
        bool eventFilter(QObject *obj, QEvent *event) override;

};

#endif // BOXMESSAGE_H
