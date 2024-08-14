#include "frame_gun_control_barrel.h"
#include "ui_frame_gun_control_barrel.h"

FrameGunControlBarrel::FrameGunControlBarrel(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::FrameGunControlBarrel)
{
    ui->setupUi(this);
}

FrameGunControlBarrel::~FrameGunControlBarrel()
{
    delete ui;
}
