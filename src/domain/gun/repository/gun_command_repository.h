#ifndef GUN_COMMAND_REPOSITORY_H
#define GUN_COMMAND_REPOSITORY_H

#include "src/domain/gun/entity/gun_barrel_entity.h"
#include "src/domain/gun/entity/gun_status_command_entity.h"

class GunCommandRepository
{
public:
    virtual void SetBarrel(const GunBarrelEntity &barrel) = 0;
    virtual void SetStatus(const GunStatusCommandEntity &status) = 0;
    virtual void SetMount(const bool &mount) = 0;
    virtual void SetSingleShot(const bool &single_shot) = 0;
    virtual void SetFire(const bool &fire) = 0;
    virtual void SetProximity(const bool &proximity) = 0;
    virtual void SetSiren(const bool &siren) = 0;
    virtual const GunBarrelEntity* GetBarrel() const = 0;
    virtual const GunStatusCommandEntity* GetStatus() const = 0;
};

#endif // GUN_COMMAND_REPOSITORY_H
