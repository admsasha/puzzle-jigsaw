#ifndef BOXMESSAGE_H
#define BOXMESSAGE_H

#include <QLabel>
#include <QEvent>

class BoxMessage : public QLabel {
    Q_OBJECT
    public:
        BoxMessage(QWidget *parent = nullptr);
        ~BoxMessage();

        void setText(QString text);

    private:
        QLabel *lblText;

        QPointF mousePosition;
        bool isMove;
        bool isWasMoved;

    protected:
        void resizeEvent(QResizeEvent */*e*/) override;
        bool eventFilter(QObject *obj, QEvent *event) override;

    signals:
        void clicked();

};

#endif // BOXMESSAGE_H
