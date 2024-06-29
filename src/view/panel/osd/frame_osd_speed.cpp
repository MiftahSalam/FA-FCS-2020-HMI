#include "frame_osd_speed.h"
#include "ui_frame_osd_speed.h"

FrameOSDSpeed::FrameOSDSpeed(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrameOSDSpeed)
{
    ui->setupUi(this);
}

FrameOSDSpeed::~FrameOSDSpeed()
{
    delete ui;
}
