#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "global.h"

#include <QMessageBox>
#include <QDateTime>
#include <QTextOption>
#include <QRect>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(&mastertimer, SIGNAL(timeout()), ui->frameBottomOSD, SLOT(on_osdTimerTimeOut()));
    connect(&mastertimer, SIGNAL(timeout()), this, SLOT(on_timeout()));
    connect(&mastertimer, SIGNAL(timeout()), ui->frameBottomGun, SLOT(on_gunTimerTimeOut()));

    mastertimer.start(1000);

    ui->gridLayout->removeWidget(ui->frameTDA);
    ui->gridLayout->removeWidget(ui->frameBottomGun);
    ui->gridLayout->removeWidget(ui->frameBottomOSD);
    ui->gridLayout->removeWidget(ui->frameBottomWAP);
    ui->gridLayout->removeWidget(ui->frameBottomClose);

    ui->gridLayout->addWidget(ui->frameTDA,0,0,6,3);
    ui->gridLayout->addWidget(ui->frameBottomWAP,0,3,6,1);
    ui->gridLayout->addWidget(ui->frameBottomGun,0,4,6,1);
    ui->gridLayout->addWidget(ui->frameBottomOSD,6,0,1,4);
    ui->gridLayout->addWidget(ui->frameBottomClose,6,4,1,1);



   // ui->label->setText(dateTimeString);

    /*
    ui->gridLayout->addWidget(ui->frameTDA,0,0,5,3);
    ui->gridLayout->addWidget(ui->frameBottomGun,0,4,5,1);
    ui->gridLayout->addWidget(ui->frameBottomWAP,0,3,5,1);
    ui->gridLayout->addWidget(ui->frameBottomOSD,5,0,1,4);
    ui->gridLayout->addWidget(ui->frameBottomClose,5,4,1,1);
    */
//    qDebug()<<ui->gridLayout_2->columnCount()<<ui->gridLayout_2->rowCount();

}

void MainWindow::on_timeout()
{
    ui->frameTDA->setHeading(ui->frameBottomOSD->getHeading());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButtonExit_clicked()
{
    QMessageBox::StandardButton clickedButton;
    clickedButton = QMessageBox::question( this, "Shutdown", "Shutdown System?",
                                           QMessageBox::Yes|QMessageBox::No );

    if( clickedButton != QMessageBox::Yes )
        return;

    close();
}

void MainWindow::setConfig(QSettings &Config)
{
    splash->show();

    QString osd = Config.value("Redis/osd", "192.168.1.2").toString();
    QString track = Config.value("Redis/track", "192.168.1.2").toString();
    QString gun = Config.value("Redis/gun", "192.168.1.2").toString();

    verbose = Config.value("Apps/verbose", false).toBool();
    qDebug()<<Q_FUNC_INFO<<"verbose"<<verbose;

    splash->showMessage("Setting up osd database connection...",Qt::AlignCenter);
    ui->frameBottomOSD->setConfig(osd);
    splash->showMessage("Setting up track database connection...",Qt::AlignCenter);
    ui->frameTDA->setConfig(track);
    splash->showMessage("Setting up gun database connection...",Qt::AlignCenter);
    ui->frameBottomGun->setConfig(gun);

    splash->finish(this);
}

