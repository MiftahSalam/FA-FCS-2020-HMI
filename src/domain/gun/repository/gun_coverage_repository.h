#ifndef GUN_COVERAGE_REPOSITORY_H
#define GUN_COVERAGE_REPOSITORY_H

#include "src/domain/gun/entity/gun_coverage_entity.h"
#include "src/domain/gun/repository/gun_base_repository.h"

class GunCoverageRepository
{
public:
    virtual void SetGunCoverage(const GunCoverageEntity &gunCov) = 0;
    virtual const GunCoverageEntity* GetGunCoverage() const = 0;
};

#endif // GUN_COVERAGE_REPOSITORY_H
