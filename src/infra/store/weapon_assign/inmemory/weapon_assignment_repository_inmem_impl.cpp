#include "weapon_assignment_repository_inmem_impl.h"

#include <QList>

WeaponAssignmentRepositoryInMemImpl* WeaponAssignmentRepositoryInMemImpl::instance{nullptr};

WeaponAssignmentRepositoryInMemImpl::WeaponAssignmentRepositoryInMemImpl()
{
    _waEntities.clear();
}

WeaponAssignmentRepository *WeaponAssignmentRepositoryInMemImpl::GetInstance()
{
    if (instance == nullptr) {
        instance = new WeaponAssignmentRepositoryInMemImpl();
    }

    return instance;
}

bool WeaponAssignmentRepositoryInMemImpl::AddAssignment(const WeaponAssign &wa)
{
    WeaponAssign* new_wa = new WeaponAssign(wa.getWeapon(), wa.getMode());
    _waEntities.insert(_waEntities.begin(),new_wa);

    return true;
}

bool WeaponAssignmentRepositoryInMemImpl::SetAssignment(const std::string &weapon, const WeaponAssign::WeaponAssignMode &mode)
{
    QList<WeaponAssign*> weapon_assign_list(_waEntities.begin(), _waEntities.end());
    foreach (auto wa, weapon_assign_list) {
        if (wa->getWeapon() == weapon) {
            wa->setMode(mode);
            return true;
        }
    }

    return false;
}

const WeaponAssign *WeaponAssignmentRepositoryInMemImpl::GetAssignment(const std::string &weapon)
{
    QList<WeaponAssign*> weapon_assign_list(_waEntities.begin(), _waEntities.end());
    foreach (auto wa, weapon_assign_list) {
        if (wa->getWeapon() == weapon) {
            return wa;
        }
    }

    return nullptr;
}

std::list<WeaponAssign *> WeaponAssignmentRepositoryInMemImpl::GetAllAssignment() const
{
    return _waEntities;
}
