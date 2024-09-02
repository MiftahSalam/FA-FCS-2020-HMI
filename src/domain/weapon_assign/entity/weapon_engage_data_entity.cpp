#include "weapon_engage_data_entity.h"

WeaponEngageDataEntity::WeaponEngageDataEntity(float azimuth, float elevation)
{
    _azimuth = azimuth;
    _elevation = elevation;
}

float WeaponEngageDataEntity::azimuth() const
{
    return _azimuth;
}

void WeaponEngageDataEntity::setAzimuth(float newAzimuth)
{
    _azimuth = newAzimuth;
}

float WeaponEngageDataEntity::elevation() const
{
    return _elevation;
}

void WeaponEngageDataEntity::setElevation(float newElevation)
{
    _elevation = newElevation;
}


