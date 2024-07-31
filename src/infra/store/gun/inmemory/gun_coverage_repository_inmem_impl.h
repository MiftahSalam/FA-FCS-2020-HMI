#ifndef GUN_COVERAGE_REPOSITORY_INMEM_IMPL_H
#define GUN_COVERAGE_REPOSITORY_INMEM_IMPL_H

#include "src/domain/gun/entity/gun_coverage_entity.h"
#include "src/domain/gun/repository/gun_coverage_repository.h"

class GunCoverageRepositoryInMemImpl: public GunCoverageRepository
{
public:
    static GunCoverageRepository *GetInstance();

    void SetGunCoverage(const GunCoverageEntity &gunCov) override;
    const GunCoverageEntity *GetGunCoverage() const override;

protected:
    GunCoverageRepositoryInMemImpl(GunCoverageEntity *entity);

private:
    static GunCoverageRepositoryInMemImpl *instance;
    GunCoverageEntity *_entity;

};

#endif // GUN_COVERAGE_REPOSITORY_INMEM_IMPL_H
