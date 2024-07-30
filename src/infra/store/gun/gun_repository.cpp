#include "gun_repository.h"
#include "src/infra/store/gun/inmemory/gun_command_repository_inmem_impl.h"
#include "src/infra/store/gun/inmemory/gun_feedback_repository_inmem_imp.h"

GunRepository::GunRepository(QObject *parent)
    : QObject{parent}
{
    repoGunCmd = GunCommandRepositoryInMemImpl::GetInstance();
    repoGunFback = GunFeedbackRepositoryInMemImp::GetInstance();
    // TODO: add repo gun feedback
}

GunCommandRepository *GunRepository::getRepoGunCmd() const
{
    return repoGunCmd;
}

GunFeedbackRepository *GunRepository::getRepoGunFeedback() const
{
    return repoGunFback;
}
