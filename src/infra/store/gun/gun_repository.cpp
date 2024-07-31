#include "gun_repository.h"
#include "src/infra/store/gun/inmemory/gun_command_repository_inmem_impl.h"
#include "src/infra/store/gun/inmemory/gun_coverage_repository_inmem_impl.h"

GunRepository::GunRepository(QObject *parent)
    : QObject{parent}
{
    repoGunCmd = GunCommandRepositoryInMemImpl::GetInstance();
    repoGunCov = GunCoverageRepositoryInMemImpl::GetInstance();
    // TODO: add repo gun feedback
}

GunCommandRepository *GunRepository::getRepoGunCmd() const
{
    return repoGunCmd;
}

GunCoverageRepository *GunRepository::getRepoGunCoverage() const
{
    return repoGunCov;
}
