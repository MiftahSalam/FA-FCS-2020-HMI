#ifndef FRAME_WEAPON_ASSIGN_H
#define FRAME_WEAPON_ASSIGN_H

#include "src/usecase/gun/gun_manager_service.h"
#include "src/usecase/weapon_assign/weapon_assign_service.h"
#include <QWidget>

namespace Ui {
class FrameWeaponAssign;
}

class FrameWeaponAssign : public QWidget
{
    Q_OBJECT

public:
    explicit FrameWeaponAssign(QWidget *parent = nullptr);
    ~FrameWeaponAssign();

private slots:
    void onComboBoxWAPWeaponChanged(int index);

    void onComboBoxWAPModeChanged(int index);

    void onGunCheck();

private:
    Ui::FrameWeaponAssign *ui;

    GunBarrelModeEntity::MODE currentGunMode;

    WeaponAssignService *_waService;
    GunManagerService *_gunService;

    void availableUiSetup(bool available);
    void resetMode();
    void initWeaponList();
    void setupDI();
};

#endif // FRAME_WEAPON_ASSIGN_H
