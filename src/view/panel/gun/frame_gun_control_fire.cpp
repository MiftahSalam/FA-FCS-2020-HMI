#include "frame_gun_control_fire.h"
#include "src/di/di.h"
#include "src/view/shared/constant_ui.h"
#include "ui_frame_gun_control_fire.h"

FrameGunControlFire::FrameGunControlFire(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::FrameGunControlFire),
    _openFire(false)
{
    ui->setupUi(this);

    connect(ui->pushButtonOpenHoldFire, &QPushButton::clicked, this, &FrameGunControlFire::OnPushButtonFireClick);

    setupDI();

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
            ui->pushButtonOpenHoldFire->setEnabled(true);
            setupUIHoldFire();
        } else {
            setupUINoFire();
        }
    }
}

void FrameGunControlFire::OnPushButtonFireClick()
{
    if (_openFire) {
        setupUIHoldFire();
    } else {
        setupUIOpenFire();
    }

    _fireService->setOpenFire("40mm", _openFire);
}

void FrameGunControlFire::setupDI()
{
    _fireService = DI::getInstance()->getServiceWeaponFiring();

    connect(_fireService, &GunFiringService::signal_FiringChange, this, &FrameGunControlFire::OnTrackAssignment);
}

void FrameGunControlFire::setupUINoFire()
{
    _openFire = false;
    ui->pushButtonOpenHoldFire->setText("");
    ui->pushButtonOpenHoldFire->setEnabled(false);
    ui->labelGunFire->setEnabled(false);
    ui->labelGunFire->setStyleSheet(COLOR_DISABLE_STYLESHEET);
    ui->labelGunFire->setText("No Fire");
}

void FrameGunControlFire::setupUIHoldFire()
{
    _openFire = false;
    ui->pushButtonOpenHoldFire->setText("Open Fire");
    ui->labelGunFire->setEnabled(true);
    ui->labelGunFire->setStyleSheet(COLOR_WARN_STYLESHEET);
    ui->labelGunFire->setText("Hold Fire");
}

void FrameGunControlFire::setupUIOpenFire()
{
    _openFire = true;
    ui->pushButtonOpenHoldFire->setText("Hold Fire");
    ui->labelGunFire->setEnabled(true);
    ui->labelGunFire->setStyleSheet(COLOR_OK_STYLESHEET);
    ui->labelGunFire->setText("Open Fire");
}
