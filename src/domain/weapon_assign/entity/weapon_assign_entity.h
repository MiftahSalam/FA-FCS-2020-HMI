#ifndef WEAPONASSIGN_H
#define WEAPONASSIGN_H

#include <string>

class WeaponAssign
{
public:
    enum WeaponAssignMode {
        NONE = 0,
        DIRECT
    };

    WeaponAssign();
    WeaponAssign(const std::string &weapon, WeaponAssignMode mode);

    std::string getWeapon() const;
    void setWeapon(const std::string &newWeapon);
    WeaponAssignMode getMode() const;
    void setMode(WeaponAssignMode newMode);

private:
    std::string weapon;
    WeaponAssignMode mode;
};

#endif // WEAPONASSIGN_H
