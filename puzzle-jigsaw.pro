######################################################################
# Automatically generated by qmake (2.01a) ?? ??? 15 19:12:31 2013
######################################################################

TEMPLATE = app
TARGET = puzzle-jigsaw

DEPENDPATH += .
INCLUDEPATH += .

CONFIG(debug, release|debug){
    MOC_DIR = .build/$${QMAKE_HOST.arch}/$${QT_VERSION}/debug
    OBJECTS_DIR = .build/$${QMAKE_HOST.arch}/$${QT_VERSION}/debug
    UI_DIR = .build/$${QMAKE_HOST.arch}/$${QT_VERSION}/debug
    RCC_DIR = .build/$${QMAKE_HOST.arch}/$${QT_VERSION}/debug
}
CONFIG(release, release|debug){
    MOC_DIR = .build/$${QMAKE_HOST.arch}/$${QT_VERSION}/release
    OBJECTS_DIR = .build/$${QMAKE_HOST.arch}/$${QT_VERSION}/release
    UI_DIR = .build/$${QMAKE_HOST.arch}/$${QT_VERSION}/release
    RCC_DIR = .build/$${QMAKE_HOST.arch}/$${QT_VERSION}/release
}
QMAKE_LINK_OBJECT_SCRIPT = .build/$${QMAKE_HOST.arch}/$${QT_VERSION}/object_script


DESTDIR = Bin
QT +=  widgets

win32:RC_FILE = puzzle-jigsaw.rc

isEmpty(PATH_USERDATA){
    win32: PATH_USERDATA=.
    unix:  PATH_USERDATA=/usr/share/puzzle-jigsaw
}
message("Set PATH_USERDATA:" $$PATH_USERDATA)
DEFINES += PATH_USERDATA="\\\""$$PATH_USERDATA"\\\""


TRANSLATIONS = $$files(langs/puzzle-jigsaw_*.ts)

###################################
########### install ###############

data_bin.path = /usr/bin/
data_bin.files = Bin/puzzle-jigsaw
INSTALLS += data_bin

data_images.path = /usr/share/puzzle-jigsaw/images/
data_images.files = images/*
INSTALLS += data_images

data_langs.path = /usr/share/puzzle-jigsaw/langs/
data_langs.files = langs/*.qm
INSTALLS += data_langs

data_samples.path = /usr/share/puzzle-jigsaw/samples
data_samples.files = samples/*
INSTALLS += data_samples

data_app.path = /usr/share/applications/
data_app.files = pkg/puzzle-jigsaw.desktop
INSTALLS += data_app

data_icons16.path = /usr/share/icons/hicolor/16x16/apps/
data_icons16.files = pkg/icons/16x16/puzzle-jigsaw.png
INSTALLS += data_icons16

data_icons32.path = /usr/share/icons/hicolor/32x32/apps/
data_icons32.files = pkg/icons/32x32/puzzle-jigsaw.png
INSTALLS += data_icons32

data_icons48.path = /usr/share/icons/hicolor/48x48/apps/
data_icons48.files = pkg/icons/48x48/puzzle-jigsaw.png
INSTALLS += data_icons48

data_iconSvg.path = /usr/share/icons/hicolor/scalable/apps/
data_iconSvg.files = images/puzzle-jigsaw.svg
INSTALLS += data_iconSvg


# Input
SOURCES += main.cpp  \
    BoxMessage.cpp \
    FormAbout.cpp \
    MainWindow.cpp \
    WidgetPreview.cpp

FORMS += \
    FormAbout.ui \
    MainWindow.ui \
    WidgetPreview.ui

HEADERS += \
    BoxMessage.h \
    FormAbout.h \
    MainWindow.h \
    WidgetPreview.h \
    config_puzzle.h
