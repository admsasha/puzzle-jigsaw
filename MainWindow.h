#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QGraphicsPixmapItem>
#include <QPainter>
#include <QPixmap>
#include <QtDebug>
#include <QEvent>
#include <QWheelEvent>
#include <QLabel>

#include "WidgetPreview.h"
#include "BoxMessage.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = nullptr);
        ~MainWindow();

    private:

        QVector<QLabel*> listItems;

        QLabel *centralWidget;
        QLabel *widgetTable;

        QLabel *selectItem;
        QLabel *background;
        QLabel *backgroundButtons;

        QPointF mousePosition;

        QPointF mousePositionWidgetTable;
        bool isMoveWidgetTable;

        QLabel *btnNewPuzzle;
        QLabel *btnSavePuzzle;
        QLabel *btnLoadPuzzle;
        QLabel *btnAlignment;
        QLabel *btnAbout;
        QLabel *btnPreview;
        QLabel *btnExit;

        QString buttonAction;

        WidgetPreview *preview;
        BoxMessage *boxMessage;

        QString puzzleFilename;
        QPixmap puzzlePixmap;

        void createPuzzle();
        void setPicturePuzzle(QLabel *item, const QString &effect="");
        bool isWon();
        void dockItem(QLabel *pItem);

        void newAlignment();
        void savePuzzle();
        void loadPuzzle();
        void about();

    protected:
        void resizeEvent(QResizeEvent *) override;
        bool eventFilter(QObject *obj, QEvent *event) override;
        void wheelEvent ( QWheelEvent * event ) override;

    private slots:
        void newPuzzle();
};

#endif // MAINWINDOW_H
