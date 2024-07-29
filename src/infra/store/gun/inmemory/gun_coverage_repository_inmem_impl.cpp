#include "gun_coverage_repository_inmem_impl.h"

GunCoverageRepositoryInMemImpl* GunCoverageRepositoryInMemImpl::instance(nullptr);

GunCoverageRepositoryInMemImpl::GunCoverageRepositoryInMemImpl(GunCoverageEntity *entity)
{
    _entity = entity;
}

GunCoverageRepository *GunCoverageRepositoryInMemImpl::GetInstance()
{
    if (instance == nullptr) {
        GunCoverageEntity* entity = new GunCoverageEntity(0, 0, 0);
        instance = new GunCoverageRepositoryInMemImpl (entity);
    }
    return instance;
}

void GunCoverageRepositoryInMemImpl::SetGunCoverage(const GunCoverageEntity &gunCov)
{
    _entity->setMaxRange(gunCov.getMax_range());
    _entity->setBlindArc(gunCov.getBlindArc());
    _entity->setGunOrientation(gunCov.getGunOrientation());

}

const GunCoverageEntity *GunCoverageRepositoryInMemImpl::GetGunCoverage() const
{
    return _entity;
}



