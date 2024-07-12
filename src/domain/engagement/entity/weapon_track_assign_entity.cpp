#include "weapon_track_assign_entity.h"

WeaponTrackAssignEntity::WeaponTrackAssignEntity(const std::string &weapon, int trackId) : weapon(weapon),
    trackId(trackId)
{}

std::string WeaponTrackAssignEntity::getWeapon() const
{
    return weapon;
}

int WeaponTrackAssignEntity::getTrackId() const
{
    return trackId;
}
