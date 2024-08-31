#include "weapon_track_assignment_repository_inmem_impl.h"
#include "qlist.h"

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
    if (GetEngagement(wta) == nullptr) {
        _wtaEntities.insert(_wtaEntities.begin(), new WeaponTrackAssignEntity(wta.getWeapon(), wta.getTrackId()));
        return true;
    }

    return false;
}

bool WeaponTrackAssignmentRepositoryInMemImpl::BreakEngagement(const WeaponTrackAssignEntity &wta)
{
    if (_wtaEntities.size() > 0) {
        QList<WeaponTrackAssignEntity*> wta_list(_wtaEntities.begin(), _wtaEntities.end());
        foreach (auto _wta, wta_list) {
            if (_wta->getTrackId() == wta.getTrackId() && _wta->getWeapon() == wta.getWeapon()) {
                _wtaEntities.remove(_wta);
                return true;
            }
        }
    }

    return false;
}

bool WeaponTrackAssignmentRepositoryInMemImpl::ClearAllEngagement()
{

}

const WeaponTrackAssignEntity *WeaponTrackAssignmentRepositoryInMemImpl::GetEngagement(const WeaponTrackAssignEntity &wts)
{
    if (_wtaEntities.size() > 0) {
        QList<WeaponTrackAssignEntity*> wta_list(_wtaEntities.begin(), _wtaEntities.end());
        foreach (auto wta, wta_list) {
            if (wta->getTrackId() == wts.getTrackId() && wta->getWeapon() == wts.getWeapon()) {
                return wta;
            }
        }
    }

    return nullptr;
}

std::list<WeaponTrackAssignEntity *> WeaponTrackAssignmentRepositoryInMemImpl::GetAllEngagement() const
{
    return _wtaEntities;
}
