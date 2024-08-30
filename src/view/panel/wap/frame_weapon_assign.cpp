#include "frame_weapon_assign.h"
#include "src/di/di.h"
#include "ui_frame_weapon_assign.h"

FrameWeaponAssign::FrameWeaponAssign(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrameWeaponAssign)
{
    ui->setupUi(this);

    setupDI();
}

FrameWeaponAssign::~FrameWeaponAssign()
{
    delete ui;
}

void FrameWeaponAssign::setupDI()
{
    _waService = DI::getInstance()->getServiceWeaponAssign();
    _gunService = DI::getInstance()->getServiceGunManager();
}
