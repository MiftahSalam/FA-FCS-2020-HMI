#include "frame_osd_mode.h"
#include "ui_frame_osd_mode.h"

FrameOSDMode::FrameOSDMode(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrameOSDMode)
{
    ui->setupUi(this);
}

FrameOSDMode::~FrameOSDMode()
{
    delete ui;
}
