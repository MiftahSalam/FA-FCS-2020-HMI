#include "osd_waterspeed_repository_inmem_impl.h"

#include <QDebug>

OSDWaterSpeedRepositoryInMemImpl* OSDWaterSpeedRepositoryInMemImpl::instance{nullptr};

OSDWaterSpeedRepositoryInMemImpl::OSDWaterSpeedRepositoryInMemImpl(OSDWaterSpeedEntity *entity)
{
    _entity = entity;
}

OSDWaterSpeedRepository *OSDWaterSpeedRepositoryInMemImpl::GetInstance()
{
    if (instance == nullptr) {
        OSDWaterSpeedEntity* entity = new OSDWaterSpeedEntity(0, 0, "", "", OSD_MODE::AUTO);
        instance = new OSDWaterSpeedRepositoryInMemImpl(entity);
    }

    return instance;
}

void OSDWaterSpeedRepositoryInMemImpl::SetWaterSpeed(const OSDWaterSpeedEntity &w_speed)
{
    _entity->setSpeed(w_speed.getSpeed());
    _entity->setSpeed(w_speed.getCourse());
}

const OSDWaterSpeedEntity *OSDWaterSpeedRepositoryInMemImpl::GetWaterSpeed() const
{
    return _entity;
}

void OSDWaterSpeedRepositoryInMemImpl::SetSource(const std::string &source)
{
    _entity->setSource(source);
}

void OSDWaterSpeedRepositoryInMemImpl::SetStatus(const std::string &status)
{
    _entity->setStatus(status);
}

void OSDWaterSpeedRepositoryInMemImpl::SetMode(const OSD_MODE &mode)
{
    _entity->setMode(mode);
}
