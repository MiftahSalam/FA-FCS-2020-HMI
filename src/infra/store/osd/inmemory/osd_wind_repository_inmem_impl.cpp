#include "osd_wind_repository_inmem_impl.h"

OSDWindRepositoryInMemImpl* OSDWindRepositoryInMemImpl::instance{nullptr};

OSDWindRepositoryInMemImpl::OSDWindRepositoryInMemImpl(OSDWindEntity *entity)
{
    _entity = entity;
}

OSDWindRepository *OSDWindRepositoryInMemImpl::GetInstance()
{
    if (instance == nullptr) {
        OSDWindEntity* entity = new OSDWindEntity(0, 0, "", "", OSD_MODE::AUTO);
        instance = new OSDWindRepositoryInMemImpl(entity);
    }
    return instance;
}

void OSDWindRepositoryInMemImpl::SetSource(const std::string &source)
{
    _entity->setSource(source);
}

void OSDWindRepositoryInMemImpl::SetStatus(const std::string &status)
{
    _entity->setStatus(status);
}

void OSDWindRepositoryInMemImpl::SetMode(const OSD_MODE &mode)
{
    _entity->setMode(mode);
}

void OSDWindRepositoryInMemImpl::SetWind(const OSDWindEntity &wind)
{
    _entity->setSpeed(wind.getSpeed());
    _entity->setDirection(wind.getDirection());
}

const OSDWindEntity *OSDWindRepositoryInMemImpl::GetWind() const
{
    return _entity;
}
