#include "FormAbout.h"
#include "ui_FormAbout.h"

#include <QUrl>
#include <QDesktopServices>

#include "config_puzzle.h"

FormAbout::FormAbout(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FormAbout)
{
    ui->setupUi(this);
    this->setFixedSize(this->width(),this->height());
    this->setWindowTitle(tr("About"));
    this->setWindowIcon(QIcon(QString(PATH_USERDATA)+"/images/puzzle.png"));

    ui->label_2->setText(tr("Version:")+" "+QString(PUZZLE_VERSION));
    ui->label_3->setText(tr("Date build:")+" "+QString(PUZZLE_DATEBUILD));
    ui->label_4->setText("© 2020 DanSoft. "+tr("All rights reserved."));
    ui->label_5->setPixmap(QPixmap(QString(PATH_USERDATA)+"/images/puzzle.png"));

    ui->label_7->setText(tr("Designers:")+" Rouslan Fialkovski (ruslan.fialkovsk@mail.ru)");

    connect(ui->pushButton,SIGNAL(clicked(bool)),this,SLOT(close()));
    connect(ui->pushButton_2,SIGNAL(clicked(bool)),this,SLOT(visit_web_site()));
    connect(ui->pushButton_3,SIGNAL(clicked(bool)),this,SLOT(visit_bitbucket()));
}

FormAbout::~FormAbout()
{
    delete ui;
}

void FormAbout::visit_web_site(){
    QDesktopServices::openUrl(QUrl("https://www.dansoft.ru/"));
}

void FormAbout::visit_bitbucket(){
    QDesktopServices::openUrl(QUrl("https://bitbucket.org/admsasha/puzzle"));
}
