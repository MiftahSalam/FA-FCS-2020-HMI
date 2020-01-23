#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

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

    /*
    ui->gridLayout->addWidget(ui->frameTDA,0,0,5,3);
    ui->gridLayout->addWidget(ui->frameBottomGun,0,4,5,1);
    ui->gridLayout->addWidget(ui->frameBottomWAP,0,3,5,1);
    ui->gridLayout->addWidget(ui->frameBottomOSD,5,0,1,4);
    ui->gridLayout->addWidget(ui->frameBottomClose,5,4,1,1);
    */
//    qDebug()<<ui->gridLayout_2->columnCount()<<ui->gridLayout_2->rowCount();

}

MainWindow::~MainWindow()
{
    delete ui;
}
