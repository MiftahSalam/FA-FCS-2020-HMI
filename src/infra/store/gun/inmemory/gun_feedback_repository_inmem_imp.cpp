#include "gun_feedback_repository_inmem_imp.h"

GunFeedbackRepositoryInMemImp *GunFeedbackRepositoryInMemImp::instance(nullptr);

GunFeedbackRepositoryInMemImp::GunFeedbackRepositoryInMemImp(GunBarrelEntity *bEntity, GunStatusFeedbackEntity *sEntity)
{
    _barrelEntity = bEntity;
    _statusEntity = sEntity;
}

GunFeedbackRepository *GunFeedbackRepositoryInMemImp::GetInstance()
{
    if (instance == nullptr) {
        GunBarrelEntity* b_entity = new GunBarrelEntity(0.0,0.0);
        GunStatusFeedbackEntity* s_entity = new GunStatusFeedbackEntity(
            false,
            false,
            false,
            false,
            false,
            false,
            false,
            false,
            false,
            false
            );
        instance = new GunFeedbackRepositoryInMemImp(b_entity, s_entity);
    }
    return instance;
}

void GunFeedbackRepositoryInMemImp::SetStatus(const GunStatusFeedbackEntity &status)
{
    _statusEntity->setOpMode(status.opMode());
    _statusEntity->setRemote(status.remote());
    _statusEntity->setMount(status.mount());
    _statusEntity->setBarrelTemperature(status.barrelTemperature());
    _statusEntity->setGunReadyToStart(status.gunReadyToStart());
    _statusEntity->setGunReadyToFire(status.gunReadyToFire());
    _statusEntity->setFireMode(status.fireMode());
    _statusEntity->setBlindArc(status.blindArc());
    _statusEntity->setMissAlignment(status.missAlignment());
    _statusEntity->setMagazine(status.magazine());

}

void GunFeedbackRepositoryInMemImp::SetOpMode(const bool &mode)
{
    _statusEntity->setOpMode(mode);
}

void GunFeedbackRepositoryInMemImp::SetRemote(const bool &remote)
{
    _statusEntity->setRemote(remote);
}

void GunFeedbackRepositoryInMemImp::SetMount(const bool &mount)
{
    _statusEntity->setMount(mount);
}

void GunFeedbackRepositoryInMemImp::SetBarrelTemperature(const bool &bTemp)
{
    _statusEntity->setBarrelTemperature(bTemp);
}

void GunFeedbackRepositoryInMemImp::SetGunReadyToStart(const bool &ready)
{
    _statusEntity->setGunReadyToStart(ready);
}

void GunFeedbackRepositoryInMemImp::SetGunReadyToFire(const bool &ready)
{
    _statusEntity->setGunReadyToFire(ready);
}

void GunFeedbackRepositoryInMemImp::SetFireMode(const bool &mode)
{
    _statusEntity->setFireMode(mode);
}

void GunFeedbackRepositoryInMemImp::SetBlindArc(const bool &blind)
{
    _statusEntity->setBlindArc(blind);
}

void GunFeedbackRepositoryInMemImp::SetMissAlign(const bool &miss)
{
    _statusEntity->setMissAlignment(miss);
}

void GunFeedbackRepositoryInMemImp::SetGunMegazine(const bool &magazine)
{
    _statusEntity->setMagazine(magazine);
}

const GunStatusFeedbackEntity *GunFeedbackRepositoryInMemImp::GetStatus() const
{
    return _statusEntity;
}

void GunFeedbackRepositoryInMemImp::SetBarrel(const float &azimuth, const float &elevation)
{
    _barrelEntity->setAzimuth(azimuth);
    _barrelEntity->setElevation(elevation);
}

const GunBarrelEntity *GunFeedbackRepositoryInMemImp::GetBarrel() const
{
    return _barrelEntity;
}
