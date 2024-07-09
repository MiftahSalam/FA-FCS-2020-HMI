#include "frame_osd_waterspeed.h"
#include "ui_frame_osd_waterspeed.h"

FrameOSDWaterSpeed::FrameOSDWaterSpeed(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrameOSDWaterSpeed)
{
    ui->setupUi(this);
}

FrameOSDWaterSpeed::~FrameOSDWaterSpeed()
{
    delete ui;
}
