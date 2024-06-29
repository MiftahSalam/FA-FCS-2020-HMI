#include "osd_position_repository_inmem_impl.h"

#include <QDebug>

OSDPositionRepositoryInMemImpl* OSDPositionRepositoryInMemImpl::instance{nullptr};

OSDPositionRepositoryInMemImpl::OSDPositionRepositoryInMemImpl(OSDPositionEntity *entity)
    : OSDBaseRepository(entity)
{
}


void OSDPositionRepositoryInMemImpl::SetEntity(const OSDPositionEntity &entity)
{
    if (_entity == nullptr) {
        qFatal("uninitiated entity: OSDPositionRepositoryInMemImpl");
    }

    OSDBaseRepository::SetEntity(entity);

    OSDPositionEntity *posEntity = static_cast<OSDPositionEntity *>(_entity);

    posEntity->setLatitude(entity.latitude());
    posEntity->setLongitude(entity.longitude());
}

const OSDBaseRepository *OSDPositionRepositoryInMemImpl::GetInstance()
{
    if (instance == nullptr) {
        OSDPositionEntity* entity = new OSDPositionEntity(-91.0, -181.0, "", "", false);
        instance = new OSDPositionRepositoryInMemImpl(entity);
    }

    return instance;
}
