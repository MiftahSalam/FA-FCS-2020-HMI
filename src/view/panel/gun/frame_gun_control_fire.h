#ifndef FRAME_GUN_CONTROL_FIRE_H
#define FRAME_GUN_CONTROL_FIRE_H

#include "src/usecase/gun/gun_firing_service.h"
#include <QFrame>

namespace Ui {
class FrameGunControlFire;
}

class FrameGunControlFire : public QFrame
{
    Q_OBJECT

public:
    explicit FrameGunControlFire(QWidget *parent = nullptr);
    ~FrameGunControlFire();

private slots:
    void OnTrackAssignment(const QString &weapon, const bool &assign);
    void OnPushButtonFireClick();

private:
    Ui::FrameGunControlFire *ui;

    GunFiringService* _fireService;
    bool _openFire;

    void setupDI();
    void setupUINoFire();
    void setupUIHoldFire();
    void setupUIOpenFire();
};

#endif // FRAME_GUN_CONTROL_FIRE_H
