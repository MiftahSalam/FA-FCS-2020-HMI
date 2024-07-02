#include "osd_repository.h"
#include "src/infra/store/osd/inmemory/osd_position_repository_inmem_impl.h"

OSDRepository::OSDRepository(QObject *parent)
    : QObject{parent}
{
    repoOSDPosition = OSDPositionRepositoryInMemImpl::GetInstance();
}

OSDPositionRepository *OSDRepository::getRepoOSDPosition() const
{
    return repoOSDPosition;
}
