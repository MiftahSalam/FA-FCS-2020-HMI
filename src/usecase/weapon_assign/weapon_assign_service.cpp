#include "weapon_assign_service.h"

WeaponAssignService::WeaponAssignService(QObject *parent, WeaponAssignmentRepository *repoWA)
    : QObject{parent}, _repoWA(repoWA)
{

}

bool WeaponAssignService::SetAssignment(const std::string &weapon, const WeaponAssign::WeaponAssignMode &mode)
{

}

const WeaponAssign *WeaponAssignService::GetAssignment(const QString &weapon)
{

}

QList<WeaponAssign *> WeaponAssignService::GetAllAssignment() const
{

}
