#include "engagement_data_repository_inmem_impl.h"

EngagementDataRepositoryInMemImpl* EngagementDataRepositoryInMemImpl::instance{nullptr};

EngagementDataRepositoryInMemImpl::EngagementDataRepositoryInMemImpl()
{
    _engagementEntities.clear();
}

EngagementDataRepository *EngagementDataRepositoryInMemImpl::GetInstance()
{
    if (instance == nullptr) {
        instance = new EngagementDataRepositoryInMemImpl();
    }

    return instance;
}

bool EngagementDataRepositoryInMemImpl::AddEngagement(const EngagementDataEntity &data)
{
    _entityEngagementData->setWeaponTrackAssignEntity(data.getWeaponTrackAssignEntity());
    _entityEngagementData->setCurrentBarrelPosition(data.getCurrentBarrelPosition());
    _entityEngagementData->setCurrentEngageableStatus(data.getCurrentEngageableStatus());
}

bool EngagementDataRepositoryInMemImpl::RemoveEngagement(const WeaponTrackAssignEntity &wta)
{
}

bool EngagementDataRepositoryInMemImpl::ClearAllEngagement()
{
}

const EngagementDataEntity *EngagementDataRepositoryInMemImpl::GetEngagement(const WeaponTrackAssignEntity &wta)
{
}

std::list<EngagementDataEntity *> EngagementDataRepositoryInMemImpl::GetAllEngagement() const
{
}
