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
}

void GunFeedbackRepositoryInMemImp::SetOpMode(const bool &mode)
{
}

void GunFeedbackRepositoryInMemImp::SetRemote(const bool &remote)
{
}

void GunFeedbackRepositoryInMemImp::SetBarrelTemperature(const bool &bTemp)
{
}

void GunFeedbackRepositoryInMemImp::SetGunReadyToStart(const bool &ready)
{
}

void GunFeedbackRepositoryInMemImp::SetGunReadyToFire(const bool &ready)
{
}

void GunFeedbackRepositoryInMemImp::SetFireMode(const bool &mode)
{
}

void GunFeedbackRepositoryInMemImp::SetBlindArc(const bool &blind)
{
}

void GunFeedbackRepositoryInMemImp::SetMissAlign(const bool &miss)
{
}

void GunFeedbackRepositoryInMemImp::SetGunMegazine(const bool &magazine)
{
}

const GunStatusFeedbackEntity *GunFeedbackRepositoryInMemImp::GetStatus() const
{
}


void GunFeedbackRepositoryInMemImp::SetBarrel(const float &azimuth, const float &elevation)
{

}

const GunBarrelEntity *GunFeedbackRepositoryInMemImp::GetBarrel() const
{
}
