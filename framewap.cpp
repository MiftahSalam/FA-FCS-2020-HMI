#include "framewap.h"
#include "ui_framewap.h"

FrameWAP::FrameWAP(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::FrameWAP)
{
    ui->setupUi(this);

    ui->tableWidgetCorrection->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidgetEngData->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

FrameWAP::~FrameWAP()
{
    delete ui;
}
