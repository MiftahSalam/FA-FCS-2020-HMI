#include "weapon_assign_service.h"
#include "src/shared/common/errors/err_object_creation.h"

WeaponAssignService *WeaponAssignService::instance = nullptr;

WeaponAssignService::WeaponAssignService(QObject *parent, WeaponAssignmentRepository *repoWA)
    : QObject{parent}, _repoWA(repoWA)
{
    AVAILABLE_WEAPONS_ASSIGN.insert(
                "40mm", QList<WeaponAssign::WeaponAssignMode>()<<WeaponAssign::NONE<<WeaponAssign::DIRECT
                );

    for (auto it = AVAILABLE_WEAPONS_ASSIGN.keyValueBegin(); it != AVAILABLE_WEAPONS_ASSIGN.keyValueEnd(); ++it) {
        _repoWA->AddAssignment(
                    WeaponAssign(it->first.toStdString(),
                                 (WeaponAssign::WeaponAssignMode)it->second.first())
                    );
    }
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

const QStringList WeaponAssignService::getAvailableWeapons() const
{
    return AVAILABLE_WEAPONS_ASSIGN.keys();
}

const QList<WeaponAssign::WeaponAssignMode> WeaponAssignService::getAvailableWeaponsAssignMode(QString weapon) const
{
    return AVAILABLE_WEAPONS_ASSIGN.value(weapon);
}

void WeaponAssignService::resetAllAssignment()
{
    auto weapon_assigns = _repoWA->GetAllAssignment();
    if (!weapon_assigns.empty()) {
        QList<WeaponAssign*> weapon_assign_list(weapon_assigns.begin(), weapon_assigns.end());
        foreach (auto wa, weapon_assign_list) {
            wa->setMode(WeaponAssign::NONE);
            emit OnAssignModeChange(QString::fromStdString(wa->getWeapon()), WeaponAssign::NONE);
        }
    }
}

bool WeaponAssignService::SetAssignment(const std::string &weapon, const WeaponAssign::WeaponAssignMode &mode)
{
    auto curMode = GetAssignment(QString::fromStdString(weapon))->getMode();
    if (curMode != mode) {
        emit OnAssignModeChange(QString::fromStdString(weapon), mode);
    }
    return _repoWA->SetAssignment(weapon, mode);
}

const WeaponAssign *WeaponAssignService::GetAssignment(const QString &weapon)
{
    return _repoWA->GetAssignment(weapon.toStdString());
}

QList<WeaponAssign *> WeaponAssignService::GetAllAssignment() const
{

}
