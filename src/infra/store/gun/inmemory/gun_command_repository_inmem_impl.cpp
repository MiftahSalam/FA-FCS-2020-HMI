#include "gun_command_repository_inmem_impl.h"

GunCommandRepositoryInMemImpl* GunCommandRepositoryInMemImpl::instance{nullptr};

GunCommandRepositoryInMemImpl::GunCommandRepositoryInMemImpl(
        GunBarrelEntity* bEntity,
        GunStatusCommandEntity* sEntity,
        GunBarrelModeEntity *modeEntity)
{
    _barrelEntity = bEntity;
    _statusEntity = sEntity;
    _modeEntity = modeEntity;
}

GunCommandRepository *GunCommandRepositoryInMemImpl::GetInstance()
{
    if (instance == nullptr) {
        GunBarrelEntity* b_entity = new GunBarrelEntity(0.0,0.0);
        GunStatusCommandEntity* s_entity = new GunStatusCommandEntity(
                    false,
                    false,
                    false,
                    false,
                    false
                    );
        GunBarrelModeEntity* m_entity = new GunBarrelModeEntity(GunBarrelModeEntity::NONE);
        instance = new GunCommandRepositoryInMemImpl(b_entity, s_entity, m_entity);
    }

    return instance;
}

void GunCommandRepositoryInMemImpl::SetStatus(const GunStatusCommandEntity &status)
{
    _statusEntity->setMount(status.mount());
    _statusEntity->setSingle_shot(status.single_shot());
    _statusEntity->setFire(status.fire());
    _statusEntity->setProximity(status.proximity());
    _statusEntity->setSiren(status.siren());
}

void GunCommandRepositoryInMemImpl::SetMount(const bool &mount)
{
    _statusEntity->setMount(mount);
}

void GunCommandRepositoryInMemImpl::SetSingleShot(const bool &single_shot)
{
    _statusEntity->setSingle_shot(single_shot);
}

void GunCommandRepositoryInMemImpl::SetFire(const bool &fire)
{
    _statusEntity->setFire(fire);
}

void GunCommandRepositoryInMemImpl::SetProximity(const bool &proximity)
{
    _statusEntity->setProximity(proximity);
}

void GunCommandRepositoryInMemImpl::SetSiren(const bool &siren)
{
    _statusEntity->setSiren(siren);
}

void GunCommandRepositoryInMemImpl::SetBarrelMode(const GunBarrelModeEntity &manual_mode)
{
    _modeEntity->setMode(manual_mode.getMode());
}

const GunBarrelModeEntity *GunCommandRepositoryInMemImpl::GetBarrelMode() const
{
    return _modeEntity;
}

const GunBarrelEntity *GunCommandRepositoryInMemImpl::GetBarrel() const
{
}

const GunStatusCommandEntity *GunCommandRepositoryInMemImpl::GetStatus() const
{
    return _statusEntity;
}

void GunCommandRepositoryInMemImpl::SetBarrel(const float &azimuth, const float &elevation)
{
}
