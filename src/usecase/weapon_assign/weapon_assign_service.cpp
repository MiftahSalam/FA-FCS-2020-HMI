#include "weapon_assign_service.h"
#include "src/shared/common/errors/err_object_creation.h"

WeaponAssignService *WeaponAssignService::instance = nullptr;

WeaponAssignService::WeaponAssignService(QObject *parent, WeaponAssignmentRepository *repoWA)
    : QObject{parent}, _repoWA(repoWA)
{

}

WeaponAssignService *WeaponAssignService::getInstance(QObject *parent, WeaponAssignmentRepository *waRepo)
{
    if (instance == nullptr)
    {
        if (waRepo == nullptr)
        {
            throw ErrObjectCreation();
        }

        instance = new WeaponAssignService(parent, waRepo);
    }

    return instance;

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
