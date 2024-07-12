#ifndef WEAPONASSIGNMENTREPOSITORYINMEMIMPL_H
#define WEAPONASSIGNMENTREPOSITORYINMEMIMPL_H

#include "src/domain/weapon_assign/repository/weapon_assignment_repository.h"

class WeaponAssignmentRepositoryInMemImpl: public WeaponAssignmentRepository
{
public:
    WeaponAssignmentRepositoryInMemImpl(WeaponAssignmentRepositoryInMemImpl &other) = delete;
    void operator=(const WeaponAssignmentRepositoryInMemImpl&) = delete;

    static WeaponAssignmentRepository *GetInstance();

    // WeaponAssignmentRepository interface
    bool AddAssignment(const WeaponAssign &wa) override;
    bool SetAssignment(const std::string &weapon, const WeaponAssign::WeaponAssignMode &mode) override;
    const WeaponAssign *GetAssignment(const std::string &weapon) override;
    std::list<WeaponAssign *> GetAllAssignment() const override;

protected:
    WeaponAssignmentRepositoryInMemImpl();

private:
    static WeaponAssignmentRepositoryInMemImpl *instance;

    std::list<WeaponAssign*> _waEntities;
};

#endif // WEAPONASSIGNMENTREPOSITORYINMEMIMPL_H
