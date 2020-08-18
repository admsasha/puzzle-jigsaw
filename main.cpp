#include <QApplication>
#include <QTranslator>
#include <QTime>

#include "config_puzzle.h"
#include "MainWindow.h"


int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

#if QT_VERSION < QT_VERSION_CHECK(5, 10, 0)
    // initialization for rnd
     qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
#endif

    QTranslator translator;
    QString locale = QLocale::system().name();

    translator.load(QString(PATH_USERDATA)+QString("/langs/puzzle-jigsaw_") + locale);
    app.installTranslator(&translator);


    MainWindow w;
    w.show();

    return app.exec();
}

