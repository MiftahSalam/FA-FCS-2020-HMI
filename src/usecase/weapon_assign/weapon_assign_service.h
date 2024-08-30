#ifndef WEAPONASSIGNSERVICE_H
#define WEAPONASSIGNSERVICE_H

#include "src/domain/weapon_assign/repository/weapon_assignment_repository.h"

#include <QObject>
#include <QMap>

class WeaponAssignService : public QObject
{
    Q_OBJECT
public:
    WeaponAssignService(WeaponAssignService &other) = delete;
    void operator=(const WeaponAssignService &) = delete;
    static WeaponAssignService *getInstance(
            QObject *parent = nullptr,
            WeaponAssignmentRepository *waRepo = nullptr);

    const QStringList getAvailableWeapons() const;
    const QList<WeaponAssign::WeaponAssignMode> getAvailableWeaponsAssignMode(QString weapon) const;
    void resetAllAssignment();
    bool SetAssignment(const std::string &weapon, const WeaponAssign::WeaponAssignMode &mode);
    const WeaponAssign* GetAssignment(const QString &weapon);
    QList<WeaponAssign*> GetAllAssignment() const;

protected:
    WeaponAssignService(
            QObject *parent = nullptr,
            WeaponAssignmentRepository *repoWA = nullptr
            );

private:
    static WeaponAssignService *instance;

    QMap<QString, QList<WeaponAssign::WeaponAssignMode>> AVAILABLE_WEAPONS_ASSIGN;

    WeaponAssignmentRepository *_repoWA;
};

#endif // WEAPONASSIGNSERVICE_H
