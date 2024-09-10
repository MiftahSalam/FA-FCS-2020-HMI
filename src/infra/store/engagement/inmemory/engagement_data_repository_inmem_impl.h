#ifndef ENGAGEMENTDATAREPOSITORYINMEMIMPL_H
#define ENGAGEMENTDATAREPOSITORYINMEMIMPL_H

#include "src/domain/engagement/repository/engagement_data_repository.h"

class EngagementDataRepositoryInMemImpl: public EngagementDataRepository
{
public:
    EngagementDataRepositoryInMemImpl(EngagementDataRepositoryInMemImpl &other) = delete;
    void operator=(const EngagementDataRepositoryInMemImpl&) = delete;

    static EngagementDataRepository *GetInstance();

    // EngagementDataRepository interface
    void UpdateEngagementBarrel(const EngagementDataEntity &data) override;
    bool AddEngagement(const EngagementDataEntity &data) override;
    bool RemoveEngagement(const WeaponTrackAssignEntity &wta) override;
    bool ClearAllEngagement() override;
    const EngagementDataEntity *GetEngagement(const WeaponTrackAssignEntity &wta) override;
    std::list<EngagementDataEntity *> GetAllEngagement() const override;

protected:
    EngagementDataRepositoryInMemImpl();

private:
    static EngagementDataRepositoryInMemImpl *instance;

    std::list<EngagementDataEntity*> _engagementEntities;
};

#endif // ENGAGEMENTDATAREPOSITORYINMEMIMPL_H
