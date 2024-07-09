#include "osd_repository.h"
#include "src/infra/store/osd/inmemory/osd_position_repository_inmem_impl.h"
#include "src/infra/store/osd/inmemory/osd_inertia_repository_inmem_impl.h"
#include "src/infra/store/osd/inmemory/osd_speed_repository_inmem_impl.h"


OSDRepository::OSDRepository(QObject *parent)
    : QObject{parent}
{
    repoOSDPosition = OSDPositionRepositoryInMemImpl::GetInstance();
    repoOSDInertia = OSDInertiaRepositoryInMemImpl::GetInstance();
    repoOSDSpeed = OSDSpeedRepositoryInMemImpl::GetInstance();
}

OSDPositionRepository *OSDRepository::getRepoOSDPosition() const
{
    return repoOSDPosition;
}

OSDInertiaRepository *OSDRepository::getRepoOSDInertia() const
{
    return repoOSDInertia;
}

OSDSpeedRepository *OSDRepository::getRepoOSDSpeed() const
{
    return repoOSDSpeed;
}


