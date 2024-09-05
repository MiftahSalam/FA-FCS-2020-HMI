#include "frame_gun_control_fire.h"
#include "src/di/di.h"
#include "src/view/shared/constant_ui.h"
#include "ui_frame_gun_control_fire.h"

FrameGunControlFire::FrameGunControlFire(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::FrameGunControlFire)
{
    ui->setupUi(this);

    setupUINoFire();
}

FrameGunControlFire::~FrameGunControlFire()
{
    delete ui;
}

void FrameGunControlFire::OnTrackAssignment(const QString &weapon, const bool &assign)
{
    if (weapon == "40mm") {
        if (assign) {
            ui->pushButtonOpenHoldFire->setEnabled(false);
            setupUIHoldFire();
        } else {
            setupUINoFire();
        }
    }
}

void FrameGunControlFire::OnPushButtonFireClick()
{

}

void FrameGunControlFire::setupDI()
{
    _fireService = DI::getInstance()->getServiceWeaponFiring();
}

void FrameGunControlFire::setupUINoFire()
{
    ui->pushButtonOpenHoldFire->setText("");
    ui->labelGunFire->setEnabled(false);
    ui->labelGunFire->setStyleSheet(COLOR_DISABLE_STYLESHEET);
    ui->labelGunFire->setText("No Fire");
}

void FrameGunControlFire::setupUIHoldFire()
{
    ui->pushButtonOpenHoldFire->setText("Open Fire");
    ui->labelGunFire->setEnabled(true);
    ui->labelGunFire->setStyleSheet(COLOR_WARN_STYLESHEET);
    ui->labelGunFire->setText("Hold Fire");
}

void FrameGunControlFire::setupUIOpenFire()
{
    ui->pushButtonOpenHoldFire->setText("Hold Fire");
    ui->labelGunFire->setEnabled(true);
    ui->labelGunFire->setStyleSheet(COLOR_OK_STYLESHEET);
    ui->labelGunFire->setText("Open Fire");
}
