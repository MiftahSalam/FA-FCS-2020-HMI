#include "weapon_track_assign_service.h"
#include "src/shared/common/errors/err_object_creation.h"

WeaponTrackAssignService* WeaponTrackAssignService::instance = nullptr;

WeaponTrackAssignService::WeaponTrackAssignService(
        HttpClientWrapper *parent,
        // TODO: add track engage cms config
//            TrackEngageCmsConfig *cmsConfig = nullptr,
        WeaponAssignmentRepository *repoWA,
        WeaponTrackAssignmentRepository *repoWTA
        ): QObject{parent}, _repoWA(repoWA), _repoWTA(repoWTA)
{
    if(parent == nullptr) {
        throw ErrObjectCreation();
    }
}

void WeaponTrackAssignService::onReplyFinished()
{

}

WeaponTrackAssignService *WeaponTrackAssignService::getInstance(
        HttpClientWrapper *httpClient,
        // TODO: add track engage cms config
//            TrackEngageCmsConfig *cmsConfig = nullptr,
        WeaponAssignmentRepository *repoWA,
        WeaponTrackAssignmentRepository *repoWTA
        )
{
    if (instance == nullptr) {
        // TODO: add track assign cms config
//        if(cmsConfig == nullptr) {
//            throw ErrObjectCreation();
//        }

        if(httpClient == nullptr) {
            throw ErrObjectCreation();
        }

        if(repoWA == nullptr) {
            throw ErrObjectCreation();
        }

        if(repoWTA == nullptr) {
            throw ErrObjectCreation();
        }

        instance = new WeaponTrackAssignService(httpClient/*, cmsConfig*/, repoWA, repoWTA);
    }

    return instance;
}

bool WeaponTrackAssignService::IsWeaponEngaged(const QString &weapon)
{
    auto wta = _repoWTA->GetAllEngagement();
    QList<WeaponTrackAssignEntity*> wta_list(wta.begin(), wta.end());
    foreach (auto wta, wta_list) {
        if (wta->getWeapon() == weapon.toStdString()) {
            return true;
        }
    }

    return false;
}

bool WeaponTrackAssignService::IsEngage(const QString &weapon, const int &trackId)
{
    return _repoWTA->GetEngagement(WeaponTrackAssignEntity(weapon.toStdString(), trackId)) != nullptr;
}

bool WeaponTrackAssignService::SetEngagement(const QString &weapon, const int &trackId)
{
    return _repoWTA->AddEngagement(WeaponTrackAssignEntity(weapon.toStdString(), trackId));
}

bool WeaponTrackAssignService::BreakEngagement(const QString &weapon, const int &trackId)
{
    // TODO: implement break request to server
    return _repoWTA->BreakEngagement(WeaponTrackAssignEntity(weapon.toStdString(), trackId));
}

void WeaponTrackAssignService::BreakEngagementTrack(const int &trackId)
{
    auto wta = _repoWTA->GetAllEngagement();
    QList<WeaponTrackAssignEntity*> wta_list(wta.begin(), wta.end());
    foreach (auto wta, wta_list) {
        if (wta->getTrackId() == trackId) {
            _repoWTA->BreakEngagement(WeaponTrackAssignEntity(wta->getWeapon(), trackId));
        }
    }
}

const WeaponTrackAssignEntity *WeaponTrackAssignService::GetEngagementTrack(const QString &weapon) const
{

}

QList<WeaponTrackAssignEntity *> WeaponTrackAssignService::GetEngagementWeapons(const int &trackId) const
{

}

QList<WeaponTrackAssignEntity *> WeaponTrackAssignService::GetAllEngagement() const
{

}
