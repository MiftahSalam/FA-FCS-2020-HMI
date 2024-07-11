#include "gun_repository.h"
#include "src/infra/store/gun/inmemory/gun_command_repository_inmem_impl.h"

GunRepository::GunRepository(QObject *parent)
    : QObject{parent}
{
    repoGunCmd = GunCommandRepositoryInMemImpl::GetInstance();
    // TODO: add repo gun feedback
}

GunCommandRepository *GunRepository::getRepoGunCmd() const
{
    return repoGunCmd;
}
