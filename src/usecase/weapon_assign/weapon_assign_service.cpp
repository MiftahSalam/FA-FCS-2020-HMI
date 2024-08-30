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

void WeaponAssignService::initAllAssignment(QStringList weapons)
{
    foreach (QString weapon, weapons) {
        _repoWA->AddAssignment(WeaponAssign(weapon.toStdString(), WeaponAssign::NONE));
    }
}

void WeaponAssignService::resetAllAssignment()
{
    auto weapon_assigns = _repoWA->GetAllAssignment();
    if (!weapon_assigns.empty()) {
        QList<WeaponAssign*> weapon_assign_list(weapon_assigns.begin(), weapon_assigns.end());
        foreach (auto wa, weapon_assign_list) {
            wa->setMode(WeaponAssign::NONE);
        }
    }
}

bool WeaponAssignService::SetAssignment(const std::string &weapon, const WeaponAssign::WeaponAssignMode &mode)
{
    return _repoWA->SetAssignment(weapon, mode);
}

const WeaponAssign *WeaponAssignService::GetAssignment(const QString &weapon)
{
    return _repoWA->GetAssignment(weapon.toStdString());
}

QList<WeaponAssign *> WeaponAssignService::GetAllAssignment() const
{

}
