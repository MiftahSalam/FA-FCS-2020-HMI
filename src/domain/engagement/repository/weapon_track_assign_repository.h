#ifndef WEAPON_TRACK_ASSIGN_REPOSITORY_H
#define WEAPON_TRACK_ASSIGN_REPOSITORY_H

#include "src/domain/engagement/entity/weapon_track_assign_entity.h"
#include <list>

class WeaponTrackAssignmentRepository
{
public:
    virtual bool AddEngagement(const WeaponTrackAssignEntity &wta) = 0;
    virtual bool BreakEngagement(const WeaponTrackAssignEntity &wta) = 0;
    virtual bool ClearAllEngagement() = 0;
    virtual const WeaponTrackAssignEntity* GetEngagement(const WeaponTrackAssignEntity &wts) = 0;
    virtual  std::list<WeaponTrackAssignEntity*> GetAllEngagement() const = 0;
};

#endif // WEAPON_TRACK_ASSIGN_REPOSITORY_H
