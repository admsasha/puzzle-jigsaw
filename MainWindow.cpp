#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QBitmap>
#include <QPainter>
#include <QFileDialog>
#include <QScrollBar>
#include <QDir>
#include <QStandardPaths>
#include <QMessageBox>
#include <QRgb>
#include <QBuffer>
#include <QScreen>
#include <QStandardPaths>
#include <QDir>
#if QT_VERSION >= QT_VERSION_CHECK(5, 10, 0)
#include <QRandomGenerator>
#endif

#include <iostream>
#include <cstdlib>

#include "config_puzzle.h"
#include "FormAbout.h"

int puzzleOrigWidth = 200;
int puzzleOrigHeight = 165;

int puzzleBorderWidth = 30;
int puzzleBorderHeight = 28;

int puzzleWidth = puzzleOrigWidth-57;       // 143
int puzzleHeight = puzzleOrigHeight-57;     // 108
int distanceForAutoBonding = 15;

// puzzle jigsaw

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    this->resize(1024,768);
    this->move((qApp->primaryScreen()->size().width()-this->width())/2,(qApp->primaryScreen()->size().height()-this->height())/2);

    this->setWindowTitle(tr("Puzzle jigsaw v%1").arg(QString(PUZZLE_VERSION)));
    this->setWindowIcon(QIcon(QString(PATH_USERDATA)+"/images/puzzle.png"));

    conf =  new QSettings(QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation)+"/puzzle-jigsaw.ini", QSettings::IniFormat);


    selectItem=nullptr;
    isMoveWidgetTable=false;

    background = new QLabel(this);
    background->setPixmap(QPixmap(QString(PATH_USERDATA)+"/images/table.png"));
    background->setGeometry(0,0,this->width(),this->height());
    background->setScaledContents(true);
    background->show();
    background->lower();

    // central widget
    centralWidget =  new QLabel(this);

    // widwet for item puzzle
    widgetTable = new QLabel(centralWidget);
    widgetTable->installEventFilter(this);
    widgetTable->setFrameShape(QFrame::Box);
    widgetTable->setGeometry(-5000,-5000,10000,10000);

    // background form buttons
    backgroundButtons = new QLabel(this);
    backgroundButtons->setPixmap(QPixmap(QString(PATH_USERDATA)+"/images/background.png"));
    backgroundButtons->setScaledContents(true);
    backgroundButtons->installEventFilter(this);


    btnNewPuzzle = new QLabel(this);
    btnNewPuzzle->setPixmap(QPixmap(QString(PATH_USERDATA)+"/images/buttons/new.png"));
    btnNewPuzzle->setToolTip(tr("Open file")+"  (Ctrl+O)");
    btnNewPuzzle->setGeometry(0,32,80,80);
    btnNewPuzzle->setScaledContents(true);
    btnNewPuzzle->installEventFilter(this);

    btnSavePuzzle = new QLabel(this);
    btnSavePuzzle->setPixmap(QPixmap(QString(PATH_USERDATA)+"/images/buttons/save.png"));
    btnSavePuzzle->setToolTip(tr("Save puzzle to file")+"  (Ctrl+S)");
    btnSavePuzzle->setScaledContents(true);
    btnSavePuzzle->setGeometry(0,124,80,80);
    btnSavePuzzle->installEventFilter(this);

    btnLoadPuzzle = new QLabel(this);
    btnLoadPuzzle->setPixmap(QPixmap(QString(PATH_USERDATA)+"/images/buttons/open.png"));
    btnLoadPuzzle->setToolTip(tr("Load puzzle from file")+"  (Ctrl+L)");
    btnLoadPuzzle->setScaledContents(true);
    btnLoadPuzzle->setGeometry(0,216,80,80);
    btnLoadPuzzle->installEventFilter(this);

    btnAlignment = new QLabel(this);
    btnAlignment->setPixmap(QPixmap(QString(PATH_USERDATA)+"/images/buttons/alignment.png"));
    btnAlignment->setToolTip(tr("Alignment puzzle")+"  (Ctrl+N)");
    btnAlignment->setScaledContents(true);
    btnAlignment->setGeometry(0,308,80,80);
    btnAlignment->installEventFilter(this);

    btnPreview = new QLabel(this);
    btnPreview->setPixmap(QPixmap(QString(PATH_USERDATA)+"/images/buttons/preview.png"));
    btnPreview->setToolTip(tr("Show/hide preview")+"  (Ctrl+P)");
    btnPreview->setGeometry(0,400,80,80);
    btnPreview->setScaledContents(true);
    btnPreview->installEventFilter(this);

    btnAbout = new QLabel(this);
    btnAbout->setPixmap(QPixmap(QString(PATH_USERDATA)+"/images/buttons/about.png"));
    btnAbout->setToolTip(tr("About")+"  (Ctrl+I)");
    btnAbout->setScaledContents(true);
    btnAbout->setGeometry(0,492,80,80);
    btnAbout->installEventFilter(this);

    btnExit = new QLabel(this);
    btnExit->setPixmap(QPixmap(QString(PATH_USERDATA)+"/images/buttons/exit.png"));
    btnExit->setToolTip(tr("Exit")+"  (Ctrl+Q)");
    btnExit->setScaledContents(true);
    btnExit->setGeometry(0,600,80,80);
    btnExit->installEventFilter(this);



    preview = new WidgetPreview(this);
    preview->hide();

    boxMessage = new BoxMessage(this);
    boxMessage->setGeometry((this->width()-700)/2-50,250,700,200);
    boxMessage->setText(tr("Choose an image\n for the puzzle"));
    boxMessage->show();
    connect(boxMessage,&BoxMessage::clicked,this,&MainWindow::newPuzzle);

}

MainWindow::~MainWindow(){

}

// Creating a puzzle from an puzzlePixmap
void MainWindow::createPuzzle(){
    this->setWindowTitle(QString(tr("Puzzle [%1]")).arg(puzzleFilename));

    int countX = puzzlePixmap.width()/puzzleWidth;
    int countY = puzzlePixmap.height()/puzzleHeight;

    QPixmap pixPreview = puzzlePixmap.copy(0,0,countX*puzzleWidth,countY*puzzleHeight);
    preview->setPicture(pixPreview);


    for (auto *item:listItems) delete item;
    listItems.clear();

    QString typePuzzle = "1";
    int zOrder=0;

    for (int x=0;x<countX;x++){
        for (int y=0;y<countY;y++){
            typePuzzle="1";     // default

            if (x % 2 != 0 and y % 2 ==0){
                typePuzzle="2";
            }
            if (x % 2 == 0 and y % 2 !=0){
                typePuzzle="2";
            }


            if (x==0){
                if (y==0){
                    typePuzzle="1-l-t";
                }else{
                    if (y % 2 == 0){
                        typePuzzle="1-l";
                    }else{
                        typePuzzle="2-l";
                    }
                }
            }
            if (y==0){
                if (x==0){
                }else{
                    if (x % 2 == 0){
                        typePuzzle="1-t";
                    }else{
                        typePuzzle="2-t";
                    }
                }
            }
            if (x==countX-1){
                if (y==0){
                    if (countX % 2 == 0){
                        typePuzzle="2-r-t";
                    }else{
                        typePuzzle="1-r-t";
                    }
                }else{
                    if ((y % 2 == 0 and countX % 2 ==0) or (y % 2 != 0 and countX % 2!=0)){
                        typePuzzle="2-r";
                    }else{
                        typePuzzle="1-r";
                    }
                }
            }
            if (y==countY-1){
                if (x==0){
                    if (countY % 2 == 0 ){
                        typePuzzle="2-l-b";
                    }else{
                        typePuzzle="1-l-b";
                    }
                }else{
                    if ((x % 2 ==0 and countY % 2 == 0) or (x % 2 !=0 and countY % 2 != 0)){
                        typePuzzle="2-b";
                    }else{
                        typePuzzle="1-b";
                    }
                }
            }
            if (x==countX-1 and y==countY-1){
                if ((countX % 2 == 0 and countY % 2 == 0) or (countX % 2 != 0 and countY % 2 != 0)){
                    typePuzzle="1-r-b";
                }else{
                    typePuzzle="2-r-b";
                }
            }



            // create item puzzle
            QLabel *puzzle = new QLabel(widgetTable);
            puzzle->setGeometry(x*(puzzleWidth+20),10+y*(puzzleHeight+20),puzzleOrigWidth,puzzleOrigHeight);
            puzzle->setScaledContents(true);
            puzzle->setProperty("cell_x",x);
            puzzle->setProperty("cell_y",y);
            puzzle->setProperty("type_puzzle",typePuzzle);
            puzzle->setProperty("zOrder",zOrder);
            setPicturePuzzle(puzzle,"effect1");
            puzzle->show();
            listItems.push_back(std::move(puzzle));
            zOrder++;
        }
    }
}


// Setting the shape of the puzzle on the image
void MainWindow::setPicturePuzzle(QLabel *item, const QString &effect){
    QImage pix_temp = puzzlePixmap.toImage();
    QPixmap pix(pix_temp.width()+puzzleOrigWidth*2,pix_temp.height()+puzzleOrigHeight*2);

    QPainter painterPix(&pix);
    painterPix.drawImage(puzzleBorderWidth,puzzleBorderHeight,pix_temp,0,0);
    painterPix.end();

    QString typePuzzle = item->property("type_puzzle").toString();
    int cell_x = item->property("cell_x").toInt();
    int cell_y = item->property("cell_y").toInt();

    QPixmap puzzle_mask(QString(PATH_USERDATA)+"/images/pieces/piece"+typePuzzle+".png");
    QImage puzzle_top(QString(PATH_USERDATA)+"/images/pieces/piece"+typePuzzle+"_"+effect+".png");

    QPixmap temp = pix.copy(cell_x*puzzleWidth,cell_y*puzzleHeight,puzzleOrigWidth,puzzleOrigHeight);
    temp.setMask(puzzle_mask.createMaskFromColor(Qt::black,Qt::MaskOutColor));
    QPainter p(&temp);
    p.drawImage(0,0,puzzle_top.copy(0,0,puzzleOrigWidth,puzzleOrigHeight),0,0);
    p.end();

    item->setPixmap(temp);

}

void MainWindow::resizeEvent(QResizeEvent */*e*/){
    centralWidget->setGeometry(0,0,this->width(),this->height());

    backgroundButtons->setGeometry(this->width()-134, 0, 134,this->height());

    btnNewPuzzle->move(this->width()-100,btnNewPuzzle->y());
    btnSavePuzzle->move(this->width()-100,btnSavePuzzle->y());
    btnLoadPuzzle->move(this->width()-100,btnLoadPuzzle->y());
    btnAlignment->move(this->width()-100,btnAlignment->y());
    btnAbout->move(this->width()-100,btnAbout->y());
    btnPreview->move(this->width()-100,btnPreview->y());
    btnExit->move(this->width()-100,btnExit->y());


    background->setGeometry(0,0,this->width(),this->height());
}


bool MainWindow::eventFilter(QObject *obj, QEvent *event){
    QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);

    if (event->type() == QEvent::MouseButtonPress){
        // bug KDE?
        if (obj==backgroundButtons or obj==btnPreview or
                obj==btnNewPuzzle or obj==btnSavePuzzle or
                obj==btnLoadPuzzle or obj==btnAlignment or
                obj==btnAbout or obj==btnExit
        ){
            buttonAction="click";
            mousePosition=QPointF(mouseEvent->globalX(),mouseEvent->globalY());
            return true;
        }

        if (obj==widgetTable){
            if(mouseEvent->buttons() == Qt::LeftButton){
                QLabel *itemPuzzle = nullptr;
                int zOrder=0;
                for (auto &it:listItems){
                    if (mouseEvent->x()>it->x() and mouseEvent->x()<it->x()+it->width()
                        and mouseEvent->y()>it->y() and mouseEvent->y()<it->y()+it->height()
                    ){
                        QString typePuzzle = it->property("type_puzzle").toString();
                        QImage puzzle_mask(QString(PATH_USERDATA)+"/images/pieces/piece"+typePuzzle+".png");
                        QRgb rgb = puzzle_mask.pixel(mouseEvent->x()- it->x(),mouseEvent->y()-it->y());
                        if (qAlpha(rgb)!=0){
                            if (it->property("zOrder").toInt()>zOrder or itemPuzzle == nullptr){
                                itemPuzzle=it;
                                zOrder=it->property("zOrder").toInt();
                            }
                        }
                    }
                }

                if (itemPuzzle!=nullptr){
                    selectItem=itemPuzzle;
                    mousePosition=QPointF(mouseEvent->globalX(),mouseEvent->globalY());
                    selectItem->raise();

                    int selectZOrder=selectItem->property("zOrder").toInt();
                    for (auto &it:listItems){
                        if (it->property("zOrder").toInt()>selectZOrder){
                            it->setProperty("zOrder",it->property("zOrder").toInt()-1);
                        }
                    }
                    selectItem->setProperty("zOrder",listItems.size());
                    setPicturePuzzle(selectItem,"effect2");
                }
            }

            if(mouseEvent->buttons() == Qt::MiddleButton or mouseEvent->buttons() == Qt::RightButton){
                mousePositionWidgetTable=QPointF(mouseEvent->globalX(),mouseEvent->globalY());
                isMoveWidgetTable=true;
            }

            return true;
        }
    }

    if (event->type() == QEvent::MouseButtonRelease){
        if (selectItem!=nullptr){
            setPicturePuzzle(selectItem,"effect1");
            dockItem(selectItem);
            if (isWon()){
                for (auto &it:listItems){
                    setPicturePuzzle(it,"");
                }

                boxMessage->setGeometry((this->width()-500)/2,(this->height()-100)/2,500,100);
                boxMessage->setText(tr("You win!"));
                boxMessage->show();
            }
        }


        if (buttonAction=="click"){
            if (obj==btnPreview){
                if (!preview->isVisible()){
                    preview->show();
                    preview->move(this->width()-preview->width()-20,this->height()-preview->height()-20);
                }else{
                    preview->hide();
                }
            }

            if (obj==btnNewPuzzle){
                newPuzzle();
            }
            if (obj==btnSavePuzzle){
                savePuzzle();
            }
            if (obj==btnLoadPuzzle){
                loadPuzzle();
            }
            if (obj==btnAlignment){
                newAlignment();
            }

            if (obj==btnAbout){
                about();
            }

            if (obj==btnExit){
                QApplication::closeAllWindows();
                close();
            }
        }

        selectItem=nullptr;
        isMoveWidgetTable=false;
    }

    if (event->type() == QEvent::MouseMove){
        if (mousePosition!=QPointF(mouseEvent->globalX(),mouseEvent->globalY())){
            buttonAction="move";
        }
        if (selectItem!=nullptr){
            selectItem->move(selectItem->x()+ (mouseEvent->globalX()-mousePosition.x()),selectItem->y()+ (mouseEvent->globalY()-mousePosition.y()));
            mousePosition=QPointF(mouseEvent->globalX(),mouseEvent->globalY());
        }
        if (isMoveWidgetTable){
            widgetTable->move(widgetTable->x()+ (mouseEvent->globalX()-mousePositionWidgetTable.x()),widgetTable->y()+ (mouseEvent->globalY()-mousePositionWidgetTable.y()));
            mousePositionWidgetTable=QPointF(mouseEvent->globalX(),mouseEvent->globalY());
        }
    }

    return QObject::eventFilter(obj, event);
}

void MainWindow::wheelEvent(QWheelEvent */*event*/){

}

void MainWindow::keyPressEvent(QKeyEvent *event){
    if (event->key()==Qt::Key_Up){
        widgetTable->move(widgetTable->x(),widgetTable->y()+32);

    }
    if (event->key()==Qt::Key_Down){
        widgetTable->move(widgetTable->x(),widgetTable->y()-32);

    }
    if (event->key()==Qt::Key_Right){
        widgetTable->move(widgetTable->x()-32,widgetTable->y());

    }
    if (event->key()==Qt::Key_Left){
        widgetTable->move(widgetTable->x()+32,widgetTable->y());
    }


    // open image
    if (event->modifiers()==Qt::ControlModifier && event->key()==Qt::Key_O){
        newPuzzle();
    }

    // save puzzle
    if (event->modifiers()==Qt::ControlModifier && event->key()==Qt::Key_S){
        savePuzzle();
    }

    // load puzzle
    if (event->modifiers()==Qt::ControlModifier && event->key()==Qt::Key_L){
        loadPuzzle();
    }

    // preview
    if (event->modifiers()==Qt::ControlModifier && event->key()==Qt::Key_P){
        if (!preview->isVisible()){
            preview->show();
            preview->move(this->width()-preview->width()-20,this->height()-preview->height()-20);
        }else{
            preview->hide();
        }
    }

    // alignment
    if (event->modifiers()==Qt::ControlModifier && event->key()==Qt::Key_N){
        newAlignment();

    }

    // about
    if (event->modifiers()==Qt::ControlModifier && event->key()==Qt::Key_I){
        about();
    }

    // exit
    if (event->modifiers()==Qt::ControlModifier && event->key()==Qt::Key_Q){
        close();
    }
}

bool MainWindow::isWon(){
    for (QLabel *item:listItems){
        int itemCellX=item->property("cell_x").toInt();
        int itemCellY=item->property("cell_y").toInt();

        for (QLabel *item2:listItems){
            int item2CellX=item2->property("cell_x").toInt();
            int item2CellY=item2->property("cell_y").toInt();

            if (itemCellX+1==item2CellX and itemCellY==item2CellY){
                if (item->pos().x()+puzzleWidth-3!=item2->pos().x() or item->pos().y()!=item2->pos().y()){
                    return false;
                }
            }
            if (itemCellX==item2CellX and itemCellY+1==item2CellY){
                if (item->pos().x()!=item2->pos().x() or item->pos().y()+puzzleHeight!=item2->pos().y()){
                    return false;
                }
            }
        }
    }

    return true;
}

void MainWindow::dockItem(QLabel *pItem){
    int selectCellX=pItem->property("cell_x").toInt();
    int selectCellY=pItem->property("cell_y").toInt();

    int minDistance = distanceForAutoBonding+1;
    int newPosItemX = pItem->x();
    int newPosItemY = pItem->y();

    for (QLabel *item:listItems){
        int itemCellX=item->property("cell_x").toInt();
        int itemCellY=item->property("cell_y").toInt();

        if (selectCellX-1==itemCellX and selectCellY==itemCellY){
            if (item->pos().x()+(puzzleWidth)+distanceForAutoBonding>pItem->pos().x()
                    and item->pos().x()+(puzzleWidth)-distanceForAutoBonding<pItem->pos().x()
                    and item->pos().y()+distanceForAutoBonding>pItem->pos().y()
                    and item->pos().y()-distanceForAutoBonding<pItem->pos().y()
            ){
                int distanse = abs(pItem->pos().x()-(item->pos().x()+(puzzleWidth))) + abs(pItem->pos().y()-item->pos().y());
                if (distanse<minDistance){
                    minDistance = distanse;
                    newPosItemX=item->pos().x()+(puzzleWidth)-3;
                    newPosItemY=item->pos().y();
                }
            }
        }

        if (selectCellX+1==itemCellX and selectCellY==itemCellY){
            if (item->pos().x()-(puzzleWidth)+distanceForAutoBonding>pItem->pos().x()
                    and item->pos().x()-(puzzleWidth)-distanceForAutoBonding<pItem->pos().x()
                    and item->pos().y()+distanceForAutoBonding>pItem->pos().y()
                    and item->pos().y()-distanceForAutoBonding<pItem->pos().y()
            ){
                int distanse = abs(pItem->pos().x()-(item->pos().x()-(puzzleWidth))) + abs(pItem->pos().y()-item->pos().y());
                if (distanse<minDistance){
                    minDistance = distanse;
                    newPosItemX=item->pos().x()-(puzzleWidth)+3;
                    newPosItemY=item->pos().y();
                }
            }
        }

        if (selectCellX==itemCellX and selectCellY-1==itemCellY){
            if (item->pos().x()+distanceForAutoBonding>pItem->pos().x()
                    and item->pos().x()-distanceForAutoBonding<pItem->pos().x()
                    and item->pos().y()+(puzzleHeight)+distanceForAutoBonding>pItem->pos().y()
                    and item->pos().y()+(puzzleHeight)-distanceForAutoBonding<pItem->pos().y()
            ){
                int distanse = abs(pItem->pos().x()-item->pos().x()) + abs(pItem->pos().y()-(item->pos().y()+(puzzleHeight)));
                if (distanse<minDistance){
                    minDistance = distanse;
                    newPosItemX=item->pos().x();
                    newPosItemY=item->pos().y()+(puzzleHeight);
                }
            }
        }

        if (selectCellX==itemCellX and selectCellY+1==itemCellY){
            if (item->pos().x()+distanceForAutoBonding>pItem->pos().x()
                    and item->pos().x()-distanceForAutoBonding<pItem->pos().x()
                    and item->pos().y()-(puzzleHeight)+distanceForAutoBonding>pItem->pos().y()
                    and item->pos().y()-(puzzleHeight)-distanceForAutoBonding<pItem->pos().y()
            ){
                int distanse = abs(pItem->pos().x()-item->pos().x()) + abs(pItem->pos().y()-(item->pos().y()-(puzzleHeight)));
                if (distanse<minDistance){
                    minDistance = distanse;
                    newPosItemX=item->pos().x();
                    newPosItemY=item->pos().y()-(puzzleHeight);
                }
            }
        }
    }

    pItem->move(newPosItemX,newPosItemY);
}

// open image from file (new game)
void MainWindow::newPuzzle(){

    QString lastPath = conf->value("global/lastpath",QString(PATH_USERDATA)+"/samples").toString();

    QString filename = QFileDialog::getOpenFileName(this,tr("open images"),lastPath,tr("Images (*.png *.xpm *.jpg *.jpeg *.tiff *.webp *.bmp)"));
    if (filename.isEmpty()) return;

    conf->setValue("global/lastpath",QFileInfo(filename).path());

    disconnect(boxMessage,&BoxMessage::clicked,this,&MainWindow::newPuzzle);


    puzzleFilename = QFileInfo(filename).fileName();
    puzzlePixmap = QPixmap(filename);

    QStringList pairs = puzzleFilename.split(".");
    pairs.pop_back();
    puzzleFilename = pairs.join(".");

    createPuzzle();
    boxMessage->hide();

    newAlignment();
}

// new alignment
void MainWindow::newAlignment(){
#if QT_VERSION >= QT_VERSION_CHECK(5, 10, 0)
    QRandomGenerator *gen = QRandomGenerator::system();
#endif

    widgetTable->setGeometry(-5000,-5000,10000,10000);

    for (auto *item:listItems){
#if QT_VERSION >= QT_VERSION_CHECK(5, 10, 0)
        quint32 x = gen->bounded(0,centralWidget->width()-puzzleWidth-100);
        quint32 y = gen->bounded(0,centralWidget->height()-puzzleHeight-100);
#else
        quint32 x=qrand()%(centralWidget->width()-puzzleWidth-100);
        quint32 y=qrand()%(centralWidget->height()-puzzleHeight-100);
#endif
        item->move(0-widgetTable->x()+x,0-widgetTable->y()+y);

        setPicturePuzzle(item,"effect1");
    }
    boxMessage->hide();
}


// save game to file
void MainWindow::savePuzzle(){
    QDir dir(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)+"/puzzle-jigsaw/");
    if (!dir.exists()){
        dir.mkpath(dir.path());
    }

    QString filename = QFileDialog::getSaveFileName(this,tr("save puzzle"),dir.path(),tr("*.puzzle"));
    if (filename.isEmpty()) return;
    if (QFileInfo(filename).suffix().isEmpty()){
        filename.append(".puzzle");
    }

    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)){
        return;
    }
    QTextStream out(&file);

    QString postions;

    out << puzzleFilename << "\n";

    QByteArray bArray;
    QBuffer buffer(&bArray);
    buffer.open(QIODevice::WriteOnly);
    puzzlePixmap.save(&buffer,"PNG");

    out << bArray.toHex() << "\n";
    for (auto &item:listItems){
        postions.append(QString::number(item->x())+","+QString::number(item->y())+';');
    }
    out << postions << "\n";
    out << widgetTable->x() << ";" << widgetTable->y() << "\n";

    file.close();

}

// load game from file
void MainWindow::loadPuzzle(){
    QDir dir(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)+"/puzzle-jigsaw/");
    if (!dir.exists()){
        dir.mkpath(dir.path());
    }

    QString filename = QFileDialog::getOpenFileName(this,tr("load puzzle"),dir.path(),tr("*.puzzle"));
    if (filename.isEmpty()) return;

    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QMessageBox::critical(this,tr("puzzle"),QString(tr("Can't open the file %1")).arg(filename));
        return;
    }

    puzzleFilename = file.readLine();
    puzzlePixmap.loadFromData(QByteArray::fromHex(file.readLine()));
    QStringList positions = QString(file.readLine()).split(";");
    QStringList positionWidgetTable = QString(file.readLine()).split(";");

    puzzleFilename.replace("\n","");


    // position widgetTable
    if (positionWidgetTable.size()==2){
        widgetTable->move(positionWidgetTable.at(0).toInt(),positionWidgetTable.at(1).toInt());
    }


    createPuzzle();

    // set position for puzzles
    for (int i=0;i<positions.size()-1;i++){
        if (i>=listItems.size()) break;
        QStringList pair = positions.at(i).split(",");
        if (pair.size()!=2) continue;

        int x = pair.at(0).toInt();
        int y = pair.at(1).toInt();

        listItems.at(i)->move(x,y);
    }

    file.close();
    boxMessage->hide();
}

void MainWindow::about(){
    FormAbout about;
    about.exec();
}
