#include "frame_track_engage.h"
#include "src/di/di.h"
#include "ui_frame_track_engage.h"

FrameTrackEngage::FrameTrackEngage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrameTrackEngage)
{
    ui->setupUi(this);

    setupDI();

    currentGunMode = _gunService->getBarrelMode();

    initWeaponList();
    resetMode();
}

FrameTrackEngage::~FrameTrackEngage()
{
    delete ui;
}

void FrameTrackEngage::resetMode()
{
    availableUiSetup(false);

    disconnect(ui->comboBoxTrackEngTN, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &FrameTrackEngage::onComboBoxTrackEngTNChange);
    disconnect(ui->comboBoxTrackEngWeapon, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &FrameTrackEngage::onComboBoxTrackEngWeaponChange);

    ui->comboBoxTrackEngTN->setCurrentIndex(0);
    ui->comboBoxTrackEngWeapon->setCurrentIndex(0);

    connect(ui->comboBoxTrackEngTN, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &FrameTrackEngage::onComboBoxTrackEngTNChange);
    connect(ui->comboBoxTrackEngWeapon, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &FrameTrackEngage::onComboBoxTrackEngWeaponChange);
}

void FrameTrackEngage::setupDI()
{
    _waService = DI::getInstance()->getServiceWeaponAssign();
    _gunService = DI::getInstance()->getServiceGunManager();

    connect(_gunService, &GunManagerService::OnBarrelModeCheck, this, &FrameTrackEngage::onGunCheck);
    connect(_waService, &WeaponAssignService::OnAssignModeChange, this, &FrameTrackEngage::onAssignModeChange);
}

void FrameTrackEngage::availableUiSetup(bool available)
{
    ui->comboBoxTrackEngTN->setEnabled(available);
    ui->comboBoxTrackEngWeapon->setEnabled(available);
    ui->pushButtonTrackEngAssign->setEnabled(available);
}

void FrameTrackEngage::noWeaponUI()
{
    disconnect(ui->comboBoxTrackEngWeapon, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &FrameTrackEngage::onComboBoxTrackEngWeaponChange);
    ui->comboBoxTrackEngTN->setEnabled(false);
    ui->comboBoxTrackEngTN->setCurrentIndex(0);
    ui->pushButtonTrackEngAssign->setEnabled(false);
    ui->pushButtonTrackEngAssign->setText("Assign/Break");
    connect(ui->comboBoxTrackEngWeapon, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &FrameTrackEngage::onComboBoxTrackEngWeaponChange);
}

void FrameTrackEngage::initWeaponList()
{
    QStringList weapons = _waService->getAvailableWeapons();
    for (int var = 0; var < weapons.size(); var++) {
        ui->comboBoxTrackEngWeapon->insertItem(var+1, weapons.at(var));
    }
}

void FrameTrackEngage::on_pushButtonTrackEngAssign_clicked()
{

}

void FrameTrackEngage::onComboBoxTrackEngTNChange(int index)
{

}

void FrameTrackEngage::onComboBoxTrackEngWeaponChange(int index)
{
    if (index > 0) {
        auto wa = _waService->GetAssignment(ui->comboBoxTrackEngWeapon->currentText());
        auto wa_mode = wa->getMode();

        disconnect(ui->comboBoxTrackEngTN, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &FrameTrackEngage::onComboBoxTrackEngTNChange);
        ui->comboBoxTrackEngTN->setCurrentIndex(0);
        connect(ui->comboBoxTrackEngTN, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &FrameTrackEngage::onComboBoxTrackEngTNChange);
        ui->comboBoxTrackEngTN->setEnabled(wa_mode == WeaponAssign::DIRECT);
        ui->pushButtonTrackEngAssign->setEnabled(wa_mode == WeaponAssign::DIRECT);
    } else {
        noWeaponUI();
    }
}

void FrameTrackEngage::onGunCheck()
{
    auto curMode = _gunService->getBarrelMode();
    if ((GunBarrelModeEntity::MODE)curMode != currentGunMode)
    {
        switch (curMode) {
        case GunBarrelModeEntity::NONE:
        case GunBarrelModeEntity::MANUAL:
            resetMode();
            break;
        case GunBarrelModeEntity::AUTO:
            availableUiSetup(true);
            onComboBoxTrackEngWeaponChange(0);
            break;
        default:
            break;
        }

        currentGunMode = curMode;
    }
}

void FrameTrackEngage::onAssignModeChange(const QString &weapon, const WeaponAssign::WeaponAssignMode &mode)
{
    if (ui->comboBoxTrackEngWeapon->currentText() == weapon) {
        disconnect(ui->comboBoxTrackEngTN, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &FrameTrackEngage::onComboBoxTrackEngTNChange);
        ui->comboBoxTrackEngTN->setCurrentIndex(0);
        ui->comboBoxTrackEngTN->setEnabled(mode == WeaponAssign::DIRECT);
        ui->pushButtonTrackEngAssign->setEnabled(mode == WeaponAssign::DIRECT);
       connect(ui->comboBoxTrackEngTN, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &FrameTrackEngage::onComboBoxTrackEngTNChange);
    }
}

