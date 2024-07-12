#include "weapon_assign_entity.h"

WeaponAssign::WeaponAssign()
{

}

WeaponAssign::WeaponAssign(const std::string &weapon, WeaponAssign::WeaponAssignMode mode) : weapon(weapon),
    mode(mode)
{}

std::string WeaponAssign::getWeapon() const
{
    return weapon;
}

void WeaponAssign::setWeapon(const std::string &newWeapon)
{
    weapon = newWeapon;
}

WeaponAssign::WeaponAssignMode WeaponAssign::getMode() const
{
    return mode;
}

void WeaponAssign::setMode(WeaponAssignMode newMode)
{
    mode = newMode;
}
