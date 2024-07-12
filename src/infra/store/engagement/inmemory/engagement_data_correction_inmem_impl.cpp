#include "engagement_data_correction_inmem_impl.h"

EngagementDataCorrectionInMemImpl* EngagementDataCorrectionInMemImpl::instance{nullptr};

EngagementDataCorrectionInMemImpl::EngagementDataCorrectionInMemImpl()
{
    _engagementCorrEntities.clear();
}

EngagementDataCorrectionInMemImpl::EngagementDataCorrectionInMemImpl(const std::list<std::string> &weapons)
{
    for (auto weapon : weapons) {
        EngagementDataCorrectionEntity* corrEntity = new EngagementDataCorrectionEntity();
        _engagementCorrEntities.insert(_engagementCorrEntities.begin(), corrEntity);
    }
}

EngagementDataCorrectionRepository *EngagementDataCorrectionInMemImpl::GetInstance()
{
    if (instance == nullptr) {
        instance = new EngagementDataCorrectionInMemImpl();
    }

    return instance;
}

int EngagementDataCorrectionInMemImpl::Generate(const std::list<std::string> &weapons)
{
    if (instance == nullptr) {
        instance = new EngagementDataCorrectionInMemImpl();
    }

    // TODO: check existing entities & compare with params
    // create not exsiting entities
    // return created entities

    return static_cast<int>(weapons.size()); //temp
}

bool EngagementDataCorrectionInMemImpl::SetCorrection(const std::string &weapon, const GunBarrelEntity &bCorr)
{
}

bool EngagementDataCorrectionInMemImpl::ResetCorrection(const std::string &weapon)
{
}

const EngagementDataCorrectionEntity *EngagementDataCorrectionInMemImpl::GetCorrection(const std::string &weapon)
{
}

std::list<EngagementDataCorrectionEntity *> EngagementDataCorrectionInMemImpl::GetAllCorrectiont() const
{
}
