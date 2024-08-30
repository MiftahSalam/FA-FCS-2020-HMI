#include "frame_weapon_assign.h"
#include "src/di/di.h"
#include "ui_frame_weapon_assign.h"

#include <QMessageBox>

FrameWeaponAssign::FrameWeaponAssign(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrameWeaponAssign)
{
    ui->setupUi(this);

    setupDI();

    currentGunMode = _gunService->getBarrelMode();

    initWeaponList();
    resetMode();
}

FrameWeaponAssign::~FrameWeaponAssign()
{
    delete ui;
}

void FrameWeaponAssign::setupDI()
{
    _waService = DI::getInstance()->getServiceWeaponAssign();
    _gunService = DI::getInstance()->getServiceGunManager();

    connect(_gunService, &GunManagerService::OnBarrelModeCheck, this, &FrameWeaponAssign::onGunCheck);
}

void FrameWeaponAssign::onComboBoxWAPWeaponChanged(int index)
{
    if (index) {
        auto wa = _waService->GetAssignment(ui->comboBoxWAPWeapon->currentText());
        if (wa) {
            auto mode = wa->getMode();
            disconnect(ui->comboBoxWAPMode, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &FrameWeaponAssign::onComboBoxWAPModeChanged);
            ui->comboBoxWAPMode->setCurrentIndex((int)mode);
            connect(ui->comboBoxWAPMode, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &FrameWeaponAssign::onComboBoxWAPModeChanged);
        }
    }
}

void FrameWeaponAssign::onComboBoxWAPModeChanged(int index)
{
    std::string weapon = ui->comboBoxWAPWeapon->currentText().toStdString();
    if (weapon != "-") {
        _waService->SetAssignment(weapon, (WeaponAssign::WeaponAssignMode)index);
    } else {
        QMessageBox::warning(this, "Warning Weapon Assign Control", QString("No weapon selected"));
    }
}

void FrameWeaponAssign::onGunCheck()
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
            break;
        default:
            break;
        }

        currentGunMode = curMode;
    }
}

void FrameWeaponAssign::availableUiSetup(bool available)
{
    ui->comboBoxWAPWeapon->setEnabled(available);
    ui->comboBoxWAPMode->setEnabled(available);
}

void FrameWeaponAssign::resetMode()
{
    availableUiSetup(false);

    disconnect(ui->comboBoxWAPWeapon, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &FrameWeaponAssign::onComboBoxWAPWeaponChanged);
    disconnect(ui->comboBoxWAPMode, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &FrameWeaponAssign::onComboBoxWAPModeChanged);

    ui->comboBoxWAPWeapon->setCurrentIndex(0);
    ui->comboBoxWAPMode->setCurrentIndex(0);

    connect(ui->comboBoxWAPWeapon, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &FrameWeaponAssign::onComboBoxWAPWeaponChanged);
    connect(ui->comboBoxWAPMode, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &FrameWeaponAssign::onComboBoxWAPModeChanged);

    _waService->resetAllAssignment();
}

void FrameWeaponAssign::initWeaponList()
{
    QStringList weapons;
    for (int var = 0; var < ui->comboBoxWAPWeapon->count(); var++) {
        if (ui->comboBoxWAPWeapon->itemText(var) != "-") {
            weapons.append(ui->comboBoxWAPWeapon->itemText(var));
        }
    }
    _waService->initAllAssignment(weapons);
}

