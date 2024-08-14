#include "frame_engage_data.h"
#include "ui_frame_engage_data.h"

FrameEngageData::FrameEngageData(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrameEngageData)
{
    ui->setupUi(this);
}

FrameEngageData::~FrameEngageData()
{
    delete ui;
}
