#ifndef WEAPONTRACKASSIGNMENTREPOSITORYINMEMIMPL_H
#define WEAPONTRACKASSIGNMENTREPOSITORYINMEMIMPL_H

#include "src/domain/engagement/repository/weapon_track_assign_repository.h"

class WeaponTrackAssignmentRepositoryInMemImpl: public WeaponTrackAssignmentRepository
{
public:
    WeaponTrackAssignmentRepositoryInMemImpl(WeaponTrackAssignmentRepositoryInMemImpl &other) = delete;
    void operator=(const WeaponTrackAssignmentRepositoryInMemImpl&) = delete;

    static WeaponTrackAssignmentRepository *GetInstance();

    // WeaponTrackAssignmentRepository interface
    bool AddEngagement(const WeaponTrackAssignEntity &wta) override;
    bool BreakEngagement(const WeaponTrackAssignEntity &wta) override;
    bool ClearAllEngagement() override;
    const WeaponTrackAssignEntity *GetEngagement(const WeaponTrackAssignEntity &wts) override;
    std::list<WeaponTrackAssignEntity *> GetAllEngagement() const override;

protected:
    WeaponTrackAssignmentRepositoryInMemImpl();

private:
    static WeaponTrackAssignmentRepositoryInMemImpl *instance;

    std::list<WeaponTrackAssignEntity*> _wtaEntities;
};

#endif // WEAPONTRACKASSIGNMENTREPOSITORYINMEMIMPL_H
