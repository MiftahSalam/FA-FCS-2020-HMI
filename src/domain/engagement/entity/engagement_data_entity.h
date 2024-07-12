#ifndef ENGAGEMENTDATAENTITY_H
#define ENGAGEMENTDATAENTITY_H

#include "src/domain/engagement/entity/weapon_track_assign_entity.h"
#include "src/domain/gun/entity/gun_barrel_entity.h"
#include <string>

class EngagementDataEntity
{
public:
    enum EngagementStatus {
        NOT_ENGAGEABLE = 0,
        ENGAGEABLE
    };

    EngagementDataEntity();
    EngagementDataEntity(const WeaponTrackAssignEntity &wta, const GunBarrelEntity &currentBarrelPosition, EngagementStatus currentEngageableStatus);

    WeaponTrackAssignEntity getWeaponTrackAssignEntity() const;
    void setWeaponTrackAssignEntity(WeaponTrackAssignEntity newWTA);
    GunBarrelEntity getCurrentBarrelPosition() const;
    void setCurrentBarrelPosition(const GunBarrelEntity &newCurrentBarrelPosition);
    EngagementStatus getCurrentEngageableStatus() const;
    void setCurrentEngageableStatus(EngagementStatus newCurrentEngageableStatus);

private:
    WeaponTrackAssignEntity weaponTrackAssignEntity;
    GunBarrelEntity currentBarrelPosition;
    EngagementStatus currentEngageableStatus;
};

#endif // ENGAGEMENTDATAENTITY_H
