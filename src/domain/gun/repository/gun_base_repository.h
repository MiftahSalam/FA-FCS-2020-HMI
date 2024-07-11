#ifndef GUN_BASE_REPOSITORY_H
#define GUN_BASE_REPOSITORY_H

#include "src/domain/gun/entity/gun_barrel_entity.h"

class GunBaseRepository
{
public:
    virtual void SetBarrel(const float &azimuth, const float &elevation) = 0;
    virtual const GunBarrelEntity* GetBarrel() const = 0;
};

#endif // GUN_BASE_REPOSITORY_H
