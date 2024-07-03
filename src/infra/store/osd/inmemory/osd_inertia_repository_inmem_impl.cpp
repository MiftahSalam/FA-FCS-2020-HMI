#include "osd_inertia_repository_inmem_impl.h"

OSDInertiaRepositoryInMemImpl* OSDInertiaRepositoryInMemImpl::instance{nullptr};

OSDInertiaRepositoryInMemImpl::OSDInertiaRepositoryInMemImpl(OSDInertiaEntity *entity)
{
    _entity = entity;
}


OSDInertiaRepository *OSDInertiaRepositoryInMemImpl::GetInstance()
{
    if (instance == nullptr) {
        OSDInertiaEntity* entity = new OSDInertiaEntity(0, 90.0, 90., "", "", OSD_MODE::AUTO);
        instance = new OSDInertiaRepositoryInMemImpl(entity);
    }

    return instance;
}

void OSDInertiaRepositoryInMemImpl::SetSource(const std::string &source)
{
    _entity->setSource(source);
}

void OSDInertiaRepositoryInMemImpl::SetStatus(const std::string &status)
{
    _entity->setStatus(status);
}

void OSDInertiaRepositoryInMemImpl::SetMode(const OSD_MODE &mode)
{
    _entity->setMode(mode);
}

void OSDInertiaRepositoryInMemImpl::SetInertia(const OSDInertiaEntity &inertia)
{
    _entity->setHeading(inertia.heading());
    _entity->setPitch(inertia.pitch());
    _entity->setRoll(inertia.roll());
}

const OSDInertiaEntity *OSDInertiaRepositoryInMemImpl::GetInertia() const
{
    return _entity;
}
