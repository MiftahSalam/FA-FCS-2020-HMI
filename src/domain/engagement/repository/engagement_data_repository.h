#ifndef ENGAGEMENT_DATA_REPOSITORY_H
#define ENGAGEMENT_DATA_REPOSITORY_H

#include "src/domain/engagement/entity/engagement_data_entity.h"
#include "src/domain/engagement/entity/weapon_track_assign_entity.h"
#include <list>

class EngagementDataRepository
{
public:
    virtual bool AddEngagement(const EngagementDataEntity &data) = 0;
    virtual bool RemoveEngagement(const WeaponTrackAssignEntity &wta) = 0;
    virtual bool ClearAllEngagement() = 0;
    virtual const EngagementDataEntity* GetEngagement(const WeaponTrackAssignEntity &wta) = 0;
    virtual std::list<EngagementDataEntity*> GetAllEngagement() const = 0;
};

#endif // ENGAGEMENT_DATA_REPOSITORY_H
