#ifndef GUN_BASE_REPOSITORY_H
#define GUN_BASE_REPOSITORY_H

#include "src/domain/gun/entity/gun_barrel_entity.h"

class GunBaseRepository
{
public:
    virtual void SetBarrel(const float &azimuth, const float &elevation) = 0;
    virtual void SetMount(const bool &mount) = 0;
    virtual void SetSingleShot(const bool &single_shot) = 0;
    virtual void SetFire(const bool &fire) = 0;
    virtual void SetProximity(const bool &proximity) = 0;
    virtual void SetSiren(const bool &siren) = 0;
    virtual const GunBarrelEntity* GetBarrel() const = 0;
};

#endif // GUN_BASE_REPOSITORY_H
