#include "weapon_track_assign_service.h"
#include "src/shared/common/errors/err_engagement.h"
#include "src/shared/common/errors/err_object_creation.h"

WeaponTrackAssignService* WeaponTrackAssignService::instance = nullptr;

WeaponTrackAssignService::WeaponTrackAssignService(QObject *parent,
                                                   TrackWeaponAssignConfig *cmsConfig,
                                                   GunCoverageRepository* repoGunCov,
                                                   TrackBaseRepository *repoTrack,
                                                   OSDInertiaRepository* repoInertia,
                                                   OSDSpeedRepository* repoSpeed,
                                                   OSDCMSInputMode* cmsOSDInputMode,
                                                   OSDStreamGyro* streamInertia,
                                                   OSDStreamSpeed* streamSpeed,
                                                   TrackWeaponEngageService *cmsEngageService,
                                                   EngagementDataCorrection40mmService *cmsEngageCorrService,
                                                   WeaponAssignmentRepository *repoWA,
                                                   WeaponTrackAssignmentRepository *repoWTA
                                                   ):
    QObject{parent},
    _cmsConfig(cmsConfig),
    _cmsEngageService(cmsEngageService),
    _cmsEngageCorrService(cmsEngageCorrService),
    _repoGunCov(repoGunCov),
    _repoTrack(repoTrack),
    _cmsOSDInputMode(cmsOSDInputMode),
    _repoInertia(repoInertia),
    _repoSpeed(repoSpeed),
    _streamInertia(streamInertia),
    _streamSpeed(streamSpeed),
    _repoWA(repoWA),
    _repoWTA(repoWTA)
{
}

WeaponTrackAssignService *WeaponTrackAssignService::getInstance(QObject *parent,
                                                                TrackWeaponAssignConfig *cmsConfig,
                                                                GunCoverageRepository *repoGunCov,
                                                                TrackBaseRepository *repoTrack,
                                                                OSDRepository* repoOSD,
                                                                OSDCMS* cmsOSD,
                                                                OSDStream* streamOSD,
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

        if(cmsOSD->getServiceOSDCMSMode() == nullptr) {
            throw ErrObjectCreation();
        }

        if(repoOSD->getRepoOSDInertia() == nullptr) {
            throw ErrObjectCreation();
        }

        if(repoOSD->getRepoOSDSpeed() == nullptr) {
            throw ErrObjectCreation();
        }

        if(streamOSD->getServiceOSDStreamGyro() == nullptr) {
            throw ErrObjectCreation();
        }

        if(streamOSD->getServiceOSDStreamSpeed() == nullptr) {
            throw ErrObjectCreation();
        }


        TrackWeaponEngageService *engageCmsService = TrackWeaponEngageService::getInstance(
                    new HttpClientWrapper(),
                    cmsConfig);

        EngagementDataCorrection40mmService* cmsEngageCorrService = EngagementDataCorrection40mmService::getInstance(
            new HttpClientWrapper(),
            cmsConfig);

        instance = new WeaponTrackAssignService(
                    parent,
                    cmsConfig,
                    repoGunCov,
                    repoTrack,
                    repoOSD->getRepoOSDInertia(),
                    repoOSD->getRepoOSDSpeed(),
                    cmsOSD->getServiceOSDCMSMode(),
                    streamOSD->getServiceOSDStreamGyro(),
                    streamOSD->getServiceOSDStreamSpeed(),
                    engageCmsService,
                    cmsEngageCorrService,
                    repoWA,
                    repoWTA);

        connect(engageCmsService, &TrackWeaponEngageService::signal_trackAssignmentResponse,
                instance, &WeaponTrackAssignService::onTrackAssignmentResponse);

        connect(cmsEngageCorrService, &EngagementDataCorrection40mmService::signal_setCorrectionResponse,
                instance, &WeaponTrackAssignService::signal_engagementCorrResponse);
    }

    return instance;
}

void WeaponTrackAssignService::CheckUpdateEngagement(const QString &weapon)
{
    if(IsWeaponEngaged(weapon)) {
        auto curEngage = GetEngagementTrack(weapon);
        if (curEngage) {
            if (!isEngageable(curEngage->getTrackId())
                    || !isHeadingSpeedValid()
                    || !isIdentityEngageable(curEngage->getTrackId())
                    ) {
                BreakEngagement(weapon, curEngage->getTrackId());
            }

            _cmsEngageService->sendAssignmentHB(TrackAssignRequest(
                                                    curEngage->getTrackId(),
                                                    weapon.toStdString()
                                                    ));
        }
    }
}

void WeaponTrackAssignService::onTrackAssignmentResponse(BaseResponse<TrackAssignResponse> resp, bool assign)
{
    if (resp.getHttpCode() == 0)
    {
        if (assign) {
            _repoWTA->AddEngagement(WeaponTrackAssignEntity(resp.getData().getWeapon(), resp.getData().getTrackId()));
        } else {
            _repoWTA->BreakEngagement(WeaponTrackAssignEntity(resp.getData().getWeapon(), resp.getData().getTrackId()));
        }
    }

    emit signal_assignmentResponseData(resp, assign);
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

bool WeaponTrackAssignService::isIdentityEngageable(const int trackId)
{
    const TrackBaseEntity* track = _repoTrack->FindOne(trackId);
    if (track) {
        return track->getCurrIdentity() == TrackUtils::HOSTILE;
    }

    return false;
}

bool WeaponTrackAssignService::isHeadingSpeedValid()
{
    bool is_inertia_auto_mode = !_cmsOSDInputMode->getDataMode()->inersia();
    bool is_speed_auto_mode = !_cmsOSDInputMode->getDataMode()->speed();
    int inertia_err_code = _streamInertia->check().getCode();

    if (is_inertia_auto_mode &&
            inertia_err_code != 0 && inertia_err_code != ERROR_CODE_OSD_DATA_PARTIALLY_INVALID.first) {
        return false;
    }

    if (is_speed_auto_mode && _streamSpeed->check().getCode() != 0) {
        return false;
    }

    return true;
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
    if (isHeadingSpeedValid()) {
        if(isIdentityEngageable(trackId)) {
            if(isEngageable(trackId)) {
                _cmsEngageService->sendAssignment(TrackAssignRequest(
                                                      trackId,
                                                      weapon.toStdString()
                                                      ), true);
            } else {
                throw ErrEngagementTrackNotEngageable();
            }
        } else {
            throw ErrEngagementInvalidIdentity();
        }
    } else {
        throw ErrEngagementInvalidOSD();
    }
}

void WeaponTrackAssignService::SetCorrection(float azimuth, float elevation)
{
    _cmsEngageCorrService->setCorrection(EngagementCorrectionSetRequest(azimuth, elevation));
}

void WeaponTrackAssignService::BreakEngagement(const QString &weapon, const int &trackId)
{
    _cmsEngageService->sendAssignment(TrackAssignRequest(
                                          trackId,
                                          weapon.toStdString()
                                          ), false);
}

void WeaponTrackAssignService::BreakEngagementTrack(const int &trackId)
{
    auto wta = _repoWTA->GetAllEngagement();
    QList<WeaponTrackAssignEntity*> wta_list(wta.begin(), wta.end());
    foreach (auto wta, wta_list) {
        if (wta->getTrackId() == trackId) {
//            _repoWTA->BreakEngagement(WeaponTrackAssignEntity(wta->getWeapon(), trackId));
            BreakEngagement(QString::fromStdString(wta->getWeapon()), trackId);
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
//            BreakEngagement(weapon, wta->getTrackId());
        }
    }
    _cmsEngageService->sendResetAssignment(weapon);
}

const WeaponTrackAssignEntity *WeaponTrackAssignService::GetEngagementTrack(const QString &weapon) const
{
    auto wta = _repoWTA->GetAllEngagement();
    QList<WeaponTrackAssignEntity*> wta_list(wta.begin(), wta.end());
    foreach (auto wta, wta_list) {
        if (wta->getWeapon() == weapon.toStdString()) {
            return wta;
        }
    }

    return nullptr;
}

const QStringList WeaponTrackAssignService::GetEngagementWeapons(const int &trackId) const
{

}

QList<WeaponTrackAssignEntity *> WeaponTrackAssignService::GetAllEngagement() const
{

}
