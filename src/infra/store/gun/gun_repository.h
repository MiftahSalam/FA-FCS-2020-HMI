#ifndef GUNREPOSITORY_H
#define GUNREPOSITORY_H

#include "src/domain/gun/repository/gun_command_repository.h"
#include "src/domain/gun/repository/gun_feedback_repository.h"
#include "src/domain/gun/repository/gun_coverage_repository.h"

#include <QObject>

class GunRepository : public QObject
{
    Q_OBJECT
public:
    explicit GunRepository(QObject *parent = nullptr);

    GunCommandRepository *getRepoGunCmd() const;
    GunFeedbackRepository *getRepoGunFeedback() const;
    GunCoverageRepository *getRepoGunCoverage() const;

private:
    GunCommandRepository *repoGunCmd;
    GunCoverageRepository *repoGunCov;
    GunFeedbackRepository *repoGunFback;

    // TODO: add repo gun feedback
};

#endif // GUNREPOSITORY_H
