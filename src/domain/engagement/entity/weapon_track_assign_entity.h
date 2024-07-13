#ifndef WEAPONTRACKASSIGN_H
#define WEAPONTRACKASSIGN_H

#include <string>

class WeaponTrackAssignEntity
{
public:
    WeaponTrackAssignEntity(const std::string &weapon, int trackId);

    std::string getWeapon() const;
    int getTrackId() const;

private:
    std::string weapon;
    int trackId;
};

#endif // WEAPONTRACKASSIGN_H
