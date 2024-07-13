#include "engagement_data_entity.h"

EngagementDataEntity::EngagementDataEntity()
    : weaponTrackAssignEntity("", -1), currentBarrelPosition(0.,0.), currentEngageableStatus(EngagementStatus::NOT_ENGAGEABLE)
{    
}

EngagementDataEntity::EngagementDataEntity(
        const WeaponTrackAssignEntity &wta,
        const GunBarrelEntity &currentBarrelPosition,
        EngagementDataEntity::EngagementStatus currentEngageableStatus
        )
    : weaponTrackAssignEntity(wta),
      currentBarrelPosition(currentBarrelPosition),
      currentEngageableStatus(currentEngageableStatus)
{}

WeaponTrackAssignEntity EngagementDataEntity::getWeaponTrackAssignEntity() const
{

}

void EngagementDataEntity::setWeaponTrackAssignEntity(WeaponTrackAssignEntity newWTA)
{

}

GunBarrelEntity EngagementDataEntity::getCurrentBarrelPosition() const
{
    return currentBarrelPosition;
}

void EngagementDataEntity::setCurrentBarrelPosition(const GunBarrelEntity &newCurrentBarrelPosition)
{
    currentBarrelPosition = newCurrentBarrelPosition;
}

EngagementDataEntity::EngagementStatus EngagementDataEntity::getCurrentEngageableStatus() const
{
    return currentEngageableStatus;
}

void EngagementDataEntity::setCurrentEngageableStatus(EngagementStatus newCurrentEngageableStatus)
{
    currentEngageableStatus = newCurrentEngageableStatus;
}
