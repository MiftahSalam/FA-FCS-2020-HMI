#include "engagement_repository.h"
#include "src/infra/store/engagement/inmemory/engagement_data_repository_inmem_impl.h"

EngagementRepository::EngagementRepository(QObject *parent): QObject (parent)
{
    repoEngageData = EngagementDataRepositoryInMemImpl::GetInstance();
}

EngagementDataRepository *EngagementRepository::getRepoEngageData() const
{
    return repoEngageData;
}
