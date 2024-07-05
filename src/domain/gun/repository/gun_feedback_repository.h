#ifndef GUN_FEEDBACK_REPOSITORY_H
#define GUN_FEEDBACK_REPOSITORY_H

#include "src/domain/gun/entity/gun_barrel_entity.h"
#include "src/domain/gun/entity/gun_status_feedback_entity.h"

class GunCommandRepository
{
public:
    virtual void SetBarrel(const GunBarrelEntity &barrel) = 0;
    virtual void SetStatus(const GunStatusFeedbackEntity &status) = 0;
    virtual void SetOpMode(const bool &mode) = 0;
    virtual void SetRemote(const bool &remote) = 0;
    virtual void SetBarrelTemperature(const bool &bTemp) = 0;
    virtual void SetGunReadyToStart(const bool &ready) = 0;
    virtual void SetGunReadyToFire(const bool &ready) = 0;
    virtual void SetFireMode(const bool &mode) = 0;
    virtual void SetBlindArc(const bool &blind) = 0;
    virtual void SetMissAlign(const bool &miss) = 0;
    virtual void SetGunMegazine(const bool &magazine) = 0;
    virtual const GunBarrelEntity* GetBarrel() const = 0;
    virtual const GunStatusFeedbackEntity* GetStatus() const = 0;
};

#endif // GUN_FEEDBACK_REPOSITORY_H
