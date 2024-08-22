#ifndef GUN_COMMAND_REPOSITORY_H
#define GUN_COMMAND_REPOSITORY_H

#include "src/domain/gun/entity/gun_barrel_mode_entity.h"
#include "src/domain/gun/entity/gun_status_command_entity.h"
#include "src/domain/gun/repository/gun_base_repository.h"

class GunCommandRepository: public GunBaseRepository
{
public:
    virtual void SetStatus(const GunStatusCommandEntity &status) = 0;
    virtual void SetMount(const bool &mount) = 0;
    virtual void SetSingleShot(const bool &single_shot) = 0;
    virtual void SetFire(const bool &fire) = 0;
    virtual void SetProximity(const bool &proximity) = 0;
    virtual void SetSiren(const bool &siren) = 0;
    virtual void SetBarrelMode(const GunBarrelModeEntity &manual_mode) = 0;
    virtual const GunStatusCommandEntity* GetStatus() const = 0;
};
#endif // GUN_COMMAND_REPOSITORY_H
