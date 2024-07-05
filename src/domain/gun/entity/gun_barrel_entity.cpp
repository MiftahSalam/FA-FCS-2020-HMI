#include "gun_barrel_entity.h"

GunBarrelEntity::GunBarrelEntity(float azimuth, float elevation)
{
    _azimuth = azimuth;
    _elevation = elevation;
}

float GunBarrelEntity::azimuth() const
{
    return _azimuth;
}

void GunBarrelEntity::setAzimuth(float newAzimuth)
{
    _azimuth = newAzimuth;
}

float GunBarrelEntity::elevation() const
{
    return _elevation;
}

void GunBarrelEntity::setElevation(float newElevation)
{
    _elevation = newElevation;
}
