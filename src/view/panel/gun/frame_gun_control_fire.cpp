#include "frame_gun_control_fire.h"
#include "ui_frame_gun_control_fire.h"

FrameGunControlFire::FrameGunControlFire(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::FrameGunControlFire)
{
    ui->setupUi(this);
}

FrameGunControlFire::~FrameGunControlFire()
{
    delete ui;
}
