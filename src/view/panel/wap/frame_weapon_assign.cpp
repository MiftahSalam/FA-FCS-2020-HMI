#include "frame_weapon_assign.h"
#include "ui_frame_weapon_assign.h"

FrameWeaponAssign::FrameWeaponAssign(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrameWeaponAssign)
{
    ui->setupUi(this);
}

FrameWeaponAssign::~FrameWeaponAssign()
{
    delete ui;
}
