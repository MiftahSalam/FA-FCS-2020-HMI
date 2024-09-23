#include "engagement_data_repository_inmem_impl.h"
#include <QList>

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

void EngagementDataRepositoryInMemImpl::UpdateEngagementBarrel(const EngagementDataEntity &data)
{
    auto engagement = GetEngagement(data.getWeaponTrackAssignEntity());
    if ( engagement != nullptr) {
        engagement->getCurrentBarrelPosition().setAzimuth(data.getCurrentBarrelPosition().azimuth());
        engagement->getCurrentBarrelPosition().setElevation(data.getCurrentBarrelPosition().elevation());
    }
}

bool EngagementDataRepositoryInMemImpl::AddEngagement(const EngagementDataEntity &data)
{
    auto wta = data.getWeaponTrackAssignEntity();
    if (GetEngagement(wta) == nullptr) {
        _engagementEntities.insert(
                    _engagementEntities.begin(),
                    new EngagementDataEntity(
                        data.getWeaponTrackAssignEntity(),
                        data.getCurrentBarrelPosition(),
                        data.getCurrentEngageableStatus()
                        )
                    );
        return true;
    }

    return false;
}

bool EngagementDataRepositoryInMemImpl::RemoveEngagement(const WeaponTrackAssignEntity &wta)
{
    if (_engagementEntities.size() > 0) {
        QList<EngagementDataEntity*> engage_list(_engagementEntities.begin(), _engagementEntities.end());
        foreach (auto engage, _engagementEntities) {
            if (engage->getWeaponTrackAssignEntity().getTrackId() == wta.getTrackId() && engage->getWeaponTrackAssignEntity().getWeapon() == wta.getWeapon()) {
                _engagementEntities.remove(engage);
                return true;
            }
        }
    }

    return false;
}

bool EngagementDataRepositoryInMemImpl::ClearAllEngagement()
{
}

const EngagementDataEntity *EngagementDataRepositoryInMemImpl::GetEngagement(const WeaponTrackAssignEntity &wta)
{
    if (_engagementEntities.size() > 0) {
        QList<EngagementDataEntity*> engagement_list(_engagementEntities.begin(), _engagementEntities.end());
        foreach (auto engagement, engagement_list) {
            if (wta.getTrackId() == engagement->getWeaponTrackAssignEntity().getTrackId()
                    && wta.getWeapon() == engagement->getWeaponTrackAssignEntity().getWeapon()) {
                return engagement;
            }
        }
    }

    return nullptr;
}

std::list<EngagementDataEntity *> EngagementDataRepositoryInMemImpl::GetAllEngagement() const
{
}
