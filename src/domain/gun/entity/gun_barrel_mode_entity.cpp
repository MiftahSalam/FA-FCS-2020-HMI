#include "gun_barrel_mode_entity.h"

GunBarrelModeEntity::GunBarrelModeEntity(bool manualMode) : manualMode(manualMode)
{}

bool GunBarrelModeEntity::getManualMode() const
{
    return manualMode;
}

void GunBarrelModeEntity::setManualMode(bool newManualMode)
{
    manualMode = newManualMode;
}
