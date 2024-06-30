#include "osd_position_repository_inmem_impl.h"

#include <QDebug>

OSDPositionRepositoryInMemImpl* OSDPositionRepositoryInMemImpl::instance{nullptr};

OSDPositionRepositoryInMemImpl::OSDPositionRepositoryInMemImpl(OSDPositionEntity *entity)
{
    _entity = entity;
}

OSDPositionRepository *OSDPositionRepositoryInMemImpl::GetInstance()
{
    if (instance == nullptr) {
        OSDPositionEntity* entity = new OSDPositionEntity(-91.0, -181.0, "", "", OSD_MODE::AUTO);
        instance = new OSDPositionRepositoryInMemImpl(entity);
    }

    return instance;
}

void OSDPositionRepositoryInMemImpl::SetPosition(const OSDPositionEntity &pos)
{
    _entity->setLatitude(pos.latitude());
    _entity->setLongitude(pos.longitude());
}

const OSDPositionEntity *OSDPositionRepositoryInMemImpl::GetPosition() const
{
    return _entity;
}

void OSDPositionRepositoryInMemImpl::SetSource(const std::string &source)
{
    _entity->setSource(source);
}

void OSDPositionRepositoryInMemImpl::SetStatus(const std::string &status)
{
    _entity->setStatus(status);
}

void OSDPositionRepositoryInMemImpl::SetMode(const OSD_MODE &mode)
{
    _entity->setMode(mode);
}
