#include "weapon_track_assignment_repository_inmem_impl.h"

WeaponTrackAssignmentRepositoryInMemImpl* WeaponTrackAssignmentRepositoryInMemImpl::instance{nullptr};

WeaponTrackAssignmentRepositoryInMemImpl::WeaponTrackAssignmentRepositoryInMemImpl()
{
    _wtaEntities.clear();
}

WeaponTrackAssignmentRepository *WeaponTrackAssignmentRepositoryInMemImpl::GetInstance()
{
    if (instance == nullptr) {
        instance = new WeaponTrackAssignmentRepositoryInMemImpl();
    }

    return instance;
}

bool WeaponTrackAssignmentRepositoryInMemImpl::AddEngagement(const WeaponTrackAssignEntity &wta)
{
}

bool WeaponTrackAssignmentRepositoryInMemImpl::BreakEngagement(const WeaponTrackAssignEntity &wta)
{
}

bool WeaponTrackAssignmentRepositoryInMemImpl::ClearAllEngagement()
{

}

const WeaponTrackAssignEntity *WeaponTrackAssignmentRepositoryInMemImpl::GetEngagement(const WeaponTrackAssignEntity &wts)
{
}

std::list<WeaponTrackAssignEntity *> WeaponTrackAssignmentRepositoryInMemImpl::GetAllEngagement() const
{
}
