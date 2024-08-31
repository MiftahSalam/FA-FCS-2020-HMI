#ifndef FRAME_TRACK_ENGAGE_H
#define FRAME_TRACK_ENGAGE_H

#include "src/usecase/gun/gun_manager_service.h"
#include "src/usecase/weapon_assign/weapon_assign_service.h"
#include <QWidget>

namespace Ui {
class FrameTrackEngage;
}

class FrameTrackEngage : public QWidget
{
    Q_OBJECT

public:
    explicit FrameTrackEngage(QWidget *parent = nullptr);
    ~FrameTrackEngage();

private slots:
    void on_pushButtonTrackEngAssign_clicked();

    void onComboBoxTrackEngTNChange(int index);

    void onComboBoxTrackEngWeaponChange(int index);

    void onGunCheck();

private:
    Ui::FrameTrackEngage *ui;

    WeaponAssignService *_waService;
    GunManagerService *_gunService;

    GunBarrelModeEntity::MODE currentGunMode;

    void availableUiSetup(bool available);
    void noWeaponUI();
    void initWeaponList();
    void resetMode();
    void setupDI();
};

#endif // FRAME_TRACK_ENGAGE_H
