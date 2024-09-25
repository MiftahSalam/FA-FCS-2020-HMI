#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QDateTime>
#include <QTextOption>
#include <QRect>

MainWindow::MainWindow(QWidget *parent, DI* di):
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _di(di)
{
    ui->setupUi(this);
    /*
    ui->gridLayout->removeWidget(ui->frameTDA);
    ui->gridLayout->removeWidget(ui->frameBottomGun);
    ui->gridLayout->removeWidget(ui->frameBottomOSD);
    ui->gridLayout->removeWidget(ui->frameBottomWAP);
    ui->gridLayout->removeWidget(ui->frameBottomClose);
    */

    ui->gridLayout->removeWidget(ui->frameTDA);
    ui->gridLayout->removeWidget(ui->frameBottomGun);
    ui->gridLayout->removeWidget(ui->frameBottomOSD);
    ui->gridLayout->removeWidget(ui->frameBottomWAP);

    /*
    ui->gridLayout->addWidget(ui->frameTDA,0,0,5,4);
    ui->gridLayout->addWidget(ui->frameBottomWAP,0,3,5,1);
    ui->gridLayout->addWidget(ui->frameBottomGun,0,4,5,1);
    ui->gridLayout->addWidget(ui->frameBottomOSD,5,0,1,4);
    ui->gridLayout->addWidget(ui->frameBottomClose,5,4,1,1);
    */
//    ui->label->setText(dateTimeString);

    /*
    ui->gridLayout->addWidget(ui->frameTDA,0,0,5,3);
    ui->gridLayout->addWidget(ui->frameBottomGun,0,4,5,1);
    ui->gridLayout->addWidget(ui->frameBottomWAP,0,3,5,1);
    ui->gridLayout->addWidget(ui->frameBottomOSD,5,0,1,4);
    ui->gridLayout->addWidget(ui->frameBottomClose,5,4,1,1);
    */

    ui->gridLayout->addWidget(ui->frameTDA,0,0,5,3);
    ui->gridLayout->addWidget(ui->frameBottomGun,0,4,6,1);
    ui->gridLayout->addWidget(ui->frameBottomWAP,0,3,5,1);
    ui->gridLayout->addWidget(ui->frameBottomOSD,5,0,1,4);

    //    qDebug()<<ui->gridLayout_2->columnCount()<<ui->gridLayout_2->rowCount();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButtonExit_clicked()
{
    auto gun_mode = _di->getServiceGunManager()->getBarrelMode();
    if (gun_mode != GunBarrelModeEntity::NONE) {
        QMessageBox::critical(this, "Application", "Cannot exit in gun mode assigned.\n"
                                                     "Please release gun assigned mode first",
                                                   QMessageBox::Ok);

        return;
    }

    QMessageBox::StandardButton clickedButton;
    clickedButton = QMessageBox::question( this, "Application", "Do you really want to exit?",
                                           QMessageBox::Yes|QMessageBox::No );

    if( clickedButton != QMessageBox::Yes )
        return;

    close();
}
