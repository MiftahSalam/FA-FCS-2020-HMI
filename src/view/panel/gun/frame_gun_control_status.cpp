#include "frame_gun_control_status.h"
#include "ui_frame_gun_control_status.h"

FrameGunControlStatus::FrameGunControlStatus(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::FrameGunControlStatus)
{
    ui->setupUi(this);
}

FrameGunControlStatus::~FrameGunControlStatus()
{
    delete ui;
}
