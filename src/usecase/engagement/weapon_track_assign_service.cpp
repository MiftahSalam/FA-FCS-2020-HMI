#include "weapon_track_assign_service.h"
#include "src/shared/common/errors/err_engagement.h"
#include "src/shared/common/errors/err_object_creation.h"

WeaponTrackAssignService* WeaponTrackAssignService::instance = nullptr;

WeaponTrackAssignService::WeaponTrackAssignService(QObject *parent,
                                                   TrackWeaponAssignConfig *cmsConfig,
                                                   GunCoverageRepository* repoGunCov,
                                                   TrackBaseRepository *repoTrack,
                                                   OSDInertiaRepository* repoInertia,
                                                   TrackWeaponEngageService *cmsEngageService,
                                                   WeaponAssignmentRepository *repoWA,
                                                   WeaponTrackAssignmentRepository *repoWTA
                                                   ):
    QObject{parent},
    _cmsConfig(cmsConfig),
    _cmsEngageService(cmsEngageService),
    _repoGunCov(repoGunCov),
    _repoTrack(repoTrack),
    _repoInertia(repoInertia),
    _repoWA(repoWA),
    _repoWTA(repoWTA)
{
}

void WeaponTrackAssignService::onReplyFinished()
{

}

bool WeaponTrackAssignService::isEngageable(const int trackId)
{
    const TrackBaseEntity* track = _repoTrack->FindOne(trackId);
    if (track) {
        float max_range = _repoGunCov->GetGunCoverage()->getMax_range()/1852.;
        float orientation = _repoGunCov->GetGunCoverage()->getGunOrientation();
        float blind_arc = _repoGunCov->GetGunCoverage()->getBlindArc();
        if (track->getRange() <= max_range) {
            float shipHeading = _repoInertia->GetInertia()->heading();
            float minBArc = 180.0 + orientation + shipHeading - (blind_arc / 2);
            float maxBArc = 180.0 + orientation + shipHeading + (blind_arc / 2);

            while (minBArc > 360 || minBArc < 0) {
                if (minBArc > 360) {
                    minBArc = minBArc - 360;
                }
                if (minBArc < 0) {
                    minBArc = minBArc + 360;
                }
            }

            while (maxBArc > 360 || maxBArc < 0) {
                if (maxBArc > 360) {
                    maxBArc = maxBArc - 360;
                }
                if (maxBArc < 0) {
                    maxBArc = maxBArc + 360;
                }
            }

            if (minBArc > maxBArc) {
                if ((track->getBearing() > maxBArc) && (track->getBearing() < minBArc)) {
                    return true;
                }
            } else {
                if ((track->getBearing() > maxBArc) || (track->getBearing() < minBArc)) {
                    return true;
                }
            }
        }
    }

    return false;
}

WeaponTrackAssignService *WeaponTrackAssignService::getInstance(QObject *parent,
                                                                TrackWeaponAssignConfig *cmsConfig,
                                                                GunCoverageRepository *repoGunCov,
                                                                TrackBaseRepository *repoTrack,
                                                                OSDInertiaRepository* repoInertia,
                                                                TrackWeaponEngageService *cmsEngageService,
                                                                WeaponAssignmentRepository *repoWA,
                                                                WeaponTrackAssignmentRepository *repoWTA
                                                                )
{
    if (instance == nullptr) {
        if(cmsConfig == nullptr) {
            throw ErrObjectCreation();
        }

        if(repoWA == nullptr) {
            throw ErrObjectCreation();
        }

        if(repoWTA == nullptr) {
            throw ErrObjectCreation();
        }

        if(repoGunCov == nullptr) {
            throw ErrObjectCreation();
        }

        if(repoTrack == nullptr) {
            throw ErrObjectCreation();
        }

        if(repoInertia == nullptr) {
            throw ErrObjectCreation();
        }

        TrackWeaponEngageService *engageCmsService = TrackWeaponEngageService::getInstance(
                    new HttpClientWrapper(),
                    cmsConfig);

        instance = new WeaponTrackAssignService(
                    parent,
                    cmsConfig,
                    repoGunCov,
                    repoTrack,
                    repoInertia,
                    engageCmsService,
                    repoWA,
                    repoWTA);
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

void WeaponTrackAssignService::SetEngagement(const QString &weapon, const int &trackId)
{
    try {
        if(isEngageable(trackId)) {
            // TODO: implement assign request to server
            if(!_repoWTA->AddEngagement(WeaponTrackAssignEntity(weapon.toStdString(), trackId))) {
                throw ErrEngagementTrackAlreadyEngaged();
            }
        } else {
            throw ErrEngagementTrackNotEngageable();
        }
    } catch (BaseError &e) {
        throw e;
    } catch (...) {
        throw ErrUnknown();
    }
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

void WeaponTrackAssignService::BreakEngagementWeapon(const QString &weapon)
{
    auto wta = _repoWTA->GetAllEngagement();
    QList<WeaponTrackAssignEntity*> wta_list(wta.begin(), wta.end());
    foreach (auto wta, wta_list) {
        if (wta->getWeapon() == weapon.toStdString()) {
            _repoWTA->BreakEngagement(WeaponTrackAssignEntity(weapon.toStdString(), wta->getTrackId()));
        }
    }
}

const WeaponTrackAssignEntity *WeaponTrackAssignService::GetEngagementTrack(const QString &weapon) const
{

}

const QStringList WeaponTrackAssignService::GetEngagementWeapons(const int &trackId) const
{

}

QList<WeaponTrackAssignEntity *> WeaponTrackAssignService::GetAllEngagement() const
{

}
