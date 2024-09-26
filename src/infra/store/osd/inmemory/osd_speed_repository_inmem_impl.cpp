#include "osd_speed_repository_inmem_impl.h"

OSDSpeedRepositoryInMemImpl* OSDSpeedRepositoryInMemImpl::instance{nullptr};

OSDSpeedRepositoryInMemImpl::OSDSpeedRepositoryInMemImpl(OSDSpeedEntity *entity)
{
    _entity = entity;
}

OSDSpeedRepository *OSDSpeedRepositoryInMemImpl::GetInstance()
{
    if (instance == nullptr) {
        OSDSpeedEntity* entity = new OSDSpeedEntity(0, 0, "", "", OSD_MODE::AUTO);
        instance = new OSDSpeedRepositoryInMemImpl(entity);
    }

    return instance;
}

void OSDSpeedRepositoryInMemImpl::SetSource(const std::string &source)
{
    _entity->setSource(source);
}

void OSDSpeedRepositoryInMemImpl::SetStatus(const std::string &status)
{
    _entity->setStatus(status);
}

void OSDSpeedRepositoryInMemImpl::SetMode(const OSD_MODE &mode)
{
    _entity->setMode(mode);
}

void OSDSpeedRepositoryInMemImpl::SetSpeed(const OSDSpeedEntity &speed)
{
    _entity->setSpeed(speed.speed());
    _entity->setCourse(speed.course());
    _entity->setMode(speed.mode());
    _entity->setSource(speed.source());
    _entity->setStatus(speed.status());
}

const OSDSpeedEntity *OSDSpeedRepositoryInMemImpl::GetSpeed() const
{
    return _entity;
}
