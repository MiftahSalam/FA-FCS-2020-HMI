#ifndef ENGAGEMENT_DATA_CORRECTION_REPOSITORY_H
#define ENGAGEMENT_DATA_CORRECTION_REPOSITORY_H

#include "src/domain/engagement/entity/engagement_data_correction_entity.h"
#include "src/domain/gun/entity/gun_barrel_entity.h"
#include <list>
#include <string>

class EngagementDataCorrectionRepository
{
public:
    virtual bool SetCorrection(const std::string &weapon, const GunBarrelEntity &bCorr) = 0;
    virtual bool ResetCorrection(const std::string &weapon) = 0;
    virtual const EngagementDataCorrectionEntity* GetCorrection(const std::string &weapon) = 0;
    virtual  std::list<EngagementDataCorrectionEntity*> GetAllCorrectiont() const = 0;
};

#endif // ENGAGEMENT_DATA_CORRECTION_REPOSITORY_H
