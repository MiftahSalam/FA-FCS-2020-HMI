#include "gun_barrel_mode_entity.h"

GunBarrelModeEntity::GunBarrelModeEntity(MODE mode) : _mode(mode)
{}

GunBarrelModeEntity::MODE GunBarrelModeEntity::getMode() const
{
    return _mode;
}

void GunBarrelModeEntity::setMode(GunBarrelModeEntity::MODE newMode)
{
    _mode = newMode;
}
