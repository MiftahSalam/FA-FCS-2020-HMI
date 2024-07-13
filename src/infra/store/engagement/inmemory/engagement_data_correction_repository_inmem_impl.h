#ifndef ENGAGEMENTDATACORRECTIONINMEMIMPL_H
#define ENGAGEMENTDATACORRECTIONINMEMIMPL_H

#include "src/domain/engagement/repository/engagement_data_correction_repository.h"

class EngagementDataCorrectionInMemImpl: public EngagementDataCorrectionRepository
{
public:
    EngagementDataCorrectionInMemImpl(EngagementDataCorrectionInMemImpl &other) = delete;
    void operator=(const EngagementDataCorrectionInMemImpl&) = delete;

    static EngagementDataCorrectionRepository *GetInstance();
    static int Generate(const std::list<std::string> &weapons);

    // EngagementDataCorrectionRepository interface
    bool SetCorrection(const std::string &weapon, const GunBarrelEntity &bCorr) override;
    bool ResetCorrection(const std::string &weapon) override;
    const EngagementDataCorrectionEntity *GetCorrection(const std::string &weapon) override;
    std::list<EngagementDataCorrectionEntity *> GetAllCorrectiont() const override;

protected:
    EngagementDataCorrectionInMemImpl();
    EngagementDataCorrectionInMemImpl(const std::list<std::string> &weapons);

private:
    static EngagementDataCorrectionInMemImpl *instance;

    std::list<EngagementDataCorrectionEntity*> _engagementCorrEntities;

};

#endif // ENGAGEMENTDATACORRECTIONINMEMIMPL_H
