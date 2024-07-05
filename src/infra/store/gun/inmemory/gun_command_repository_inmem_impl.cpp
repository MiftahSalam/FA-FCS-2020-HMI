#include "gun_command_repository_inmem_impl.h"

GunCommandRepositoryInMemImpl* GunCommandRepositoryInMemImpl::instance{nullptr};

GunCommandRepositoryInMemImpl::GunCommandRepositoryInMemImpl(
        GunBarrelEntity* bEntity, GunStatusCommandEntity* sEntity)
{
    _barrelEntity = bEntity;
    _statusEntity = sEntity;
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
        instance = new GunCommandRepositoryInMemImpl(b_entity, s_entity);
    }

    return instance;
}


void GunCommandRepositoryInMemImpl::SetBarrel(const GunBarrelEntity &barrel)
{

}

void GunCommandRepositoryInMemImpl::SetStatus(const GunStatusCommandEntity &status)
{
}

void GunCommandRepositoryInMemImpl::SetMount(const bool &mount)
{
}

void GunCommandRepositoryInMemImpl::SetSingleShot(const bool &single_shot)
{
}

void GunCommandRepositoryInMemImpl::SetFire(const bool &fire)
{
}

void GunCommandRepositoryInMemImpl::SetProximity(const bool &proximity)
{
}

void GunCommandRepositoryInMemImpl::SetSiren(const bool &siren)
{
}

const GunBarrelEntity *GunCommandRepositoryInMemImpl::GetBarrel() const
{
}

const GunStatusCommandEntity *GunCommandRepositoryInMemImpl::GetStatus() const
{
}
