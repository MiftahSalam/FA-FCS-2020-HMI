#ifndef WEAPONASSIGNSERVICE_H
#define WEAPONASSIGNSERVICE_H

#include "src/domain/weapon_assign/repository/weapon_assignment_repository.h"

#include <QObject>

class WeaponAssignService : public QObject
{
    Q_OBJECT
public:
    explicit WeaponAssignService(
            QObject *parent = nullptr,
            WeaponAssignmentRepository *repoWA = nullptr
            );

    bool SetAssignment(const std::string &weapon, const WeaponAssign::WeaponAssignMode &mode);
    const WeaponAssign* GetAssignment(const QString &weapon);
    QList<WeaponAssign*> GetAllAssignment() const;

private:
    WeaponAssignmentRepository *_repoWA;
};

#endif // WEAPONASSIGNSERVICE_H
