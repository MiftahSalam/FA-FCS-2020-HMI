#ifndef ENGAGEMENTDATACORRECTIONENTITY_H
#define ENGAGEMENTDATACORRECTIONENTITY_H

#include "src/domain/gun/entity/gun_barrel_entity.h"
#include <string>

class EngagementDataCorrectionEntity
{
public:
    EngagementDataCorrectionEntity();
    EngagementDataCorrectionEntity(const std::string &weapon, const GunBarrelEntity &barrelPositionCorrection);

    std::string getWeapon() const;
    void setWeapon(const std::string &newWeapon);
    GunBarrelEntity getBarrelPositionCorrection() const;
    void setBarrelPositionCorrection(const GunBarrelEntity &newBarrelPositionCorrection);

private:
    std::string weapon;
    GunBarrelEntity barrelPositionCorrection;
};

#endif // ENGAGEMENTDATACORRECTIONENTITY_H
