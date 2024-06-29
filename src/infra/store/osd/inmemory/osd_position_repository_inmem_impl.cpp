#include "osd_position_repository_inmem_impl.h"

#include <QDebug>

OSDPositionRepositoryInMemImpl* OSDPositionRepositoryInMemImpl::instance{nullptr};

OSDPositionRepositoryInMemImpl::OSDPositionRepositoryInMemImpl(OSDPositionEntity *entity)
    : OSDBaseRepositoryInMemImpl(entity)
{
}


void OSDPositionRepositoryInMemImpl::SetEntity(const OSDPositionEntity &entity)
{
    if (_entity == nullptr) {
        qFatal("uninitiated entity: OSDPositionRepositoryInMemImpl");
    }

    OSDBaseRepositoryInMemImpl::SetEntity(entity);

    OSDPositionEntity *posEntity = static_cast<OSDPositionEntity *>(_entity);

    posEntity->setLatitude(entity.latitude());
    posEntity->setLongitude(entity.longitude());
}

OSDBaseRepository *OSDPositionRepositoryInMemImpl::GetInstance()
{
    if (instance == nullptr) {
        OSDPositionEntity* entity = new OSDPositionEntity(-91.0, -181.0, "", "", OSD_MODE::AUTO);
        instance = new OSDPositionRepositoryInMemImpl(entity);
    }

    return instance;
}
