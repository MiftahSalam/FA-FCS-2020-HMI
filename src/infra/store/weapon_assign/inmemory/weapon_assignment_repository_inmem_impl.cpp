#include "weapon_assignment_repository_inmem_impl.h"

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
}

bool WeaponAssignmentRepositoryInMemImpl::SetAssignment(const std::string &weapon, const WeaponAssign::WeaponAssignMode &mode)
{
}

const WeaponAssign *WeaponAssignmentRepositoryInMemImpl::GetAssignment(const std::string &weapon)
{
}

std::list<WeaponAssign *> WeaponAssignmentRepositoryInMemImpl::GetAllAssignment() const
{
}
