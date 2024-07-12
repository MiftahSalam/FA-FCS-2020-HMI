#ifndef WEAPON_ASSIGNMENT_REPOSITORY_H
#define WEAPON_ASSIGNMENT_REPOSITORY_H

#include "src/domain/weapon_assign/entity/weapon_assign_entity.h"
#include <list>

class WeaponAssignmentRepository
{
public:
    virtual bool AddAssignment(const WeaponAssign &wa) = 0;
    virtual bool SetAssignment(const std::string &weapon, const WeaponAssign::WeaponAssignMode &mode) = 0;
    virtual const WeaponAssign* GetAssignment(const std::string &weapon) = 0;
    virtual  std::list<WeaponAssign*> GetAllAssignment() const = 0;
};
#endif // WEAPON_ASSIGNMENT_REPOSITORY_H
