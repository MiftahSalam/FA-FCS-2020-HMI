#include "engagement_data_correction_entity.h"

EngagementDataCorrectionEntity::EngagementDataCorrectionEntity()
    : weapon(""), barrelPositionCorrection(0.,0.)
{

}

EngagementDataCorrectionEntity::EngagementDataCorrectionEntity(const std::string &weapon, const GunBarrelEntity &barrelPositionCorrection) : weapon(weapon),
    barrelPositionCorrection(barrelPositionCorrection)
{}

std::string EngagementDataCorrectionEntity::getWeapon() const
{
    return weapon;
}

void EngagementDataCorrectionEntity::setWeapon(const std::string &newWeapon)
{
    weapon = newWeapon;
}

GunBarrelEntity EngagementDataCorrectionEntity::getBarrelPositionCorrection() const
{
    return barrelPositionCorrection;
}

void EngagementDataCorrectionEntity::setBarrelPositionCorrection(const GunBarrelEntity &newBarrelPositionCorrection)
{
    barrelPositionCorrection = newBarrelPositionCorrection;
}
