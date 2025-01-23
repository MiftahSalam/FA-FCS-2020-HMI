#ifndef WEAPONTRACKASSIGNSERVICE_H
#define WEAPONTRACKASSIGNSERVICE_H

#include "src/domain/engagement/repository/weapon_track_assign_repository.h"
#include "src/domain/gun/repository/gun_coverage_repository.h"
#include "src/domain/osd/repository/osd_inertia_repository.h"
#include "src/domain/track/repository/track_base_repository.h"
#include "src/domain/weapon_assign/repository/weapon_assignment_repository.h"
#include "src/shared/config/track_weapon_assign_config.h"
#include "src/usecase/engagement/cms/track_weapon_engage_service.h"
#include "src/usecase/engagement/cms/engagement_data_correction_40mm_service.h"
#include "src/infra/core/osd/stream/osd_stream.h"
#include <QObject>

class WeaponTrackAssignService : public QObject
{
    Q_OBJECT
public:
    WeaponTrackAssignService(WeaponTrackAssignService &other) = delete;
    void operator=(const WeaponTrackAssignService&) = delete;
    static WeaponTrackAssignService* getInstance(QObject *parent = nullptr,
                                                 TrackWeaponAssignConfig *cmsConfig = nullptr,
                                                 GunCoverageRepository* repoGunCov = nullptr,
                                                 TrackBaseRepository* repoTrack = nullptr,
                                                 OSDRepository* _repoOSD = nullptr,
                                                 OSDCMS* cmsOSD = nullptr,
                                                 OSDStream* streamOSD = nullptr,
                                                 WeaponAssignmentRepository *repoWA = nullptr,
                                                 WeaponTrackAssignmentRepository *repoWTA = nullptr
            );

    void CheckUpdateEngagement(const QString &weapon);
    bool IsWeaponEngaged(const QString &weapon);
    bool IsEngage(const QString &weapon, const int &trackId);
    void SetEngagement(const QString &weapon, const int &trackId);
    void SetCorrection(float azimuth, float elevation);
    void BreakEngagement(const QString &weapon, const int &trackId);
    void BreakEngagementTrack(const int &trackId);
    void BreakEngagementWeapon(const QString &weapon);
    const WeaponTrackAssignEntity* GetEngagementTrack(const QString &weapon) const;
    const QStringList GetEngagementWeapons(const int &trackId) const;
    QList<WeaponTrackAssignEntity*> GetAllEngagement() const;

signals:
    void signal_assignmentResponseData(BaseResponse<TrackAssignResponse> response, bool assign);
    void signal_engagementCorrResponse(BaseResponse<EngagementCorrectionSetResponse> response);

protected:
    WeaponTrackAssignService(
            QObject *parent = nullptr,
            TrackWeaponAssignConfig *cmsConfig = nullptr,
            GunCoverageRepository* repoGunCov = nullptr,
            TrackBaseRepository* repoTrack = nullptr,
            OSDInertiaRepository* repoInertia = nullptr,
            OSDSpeedRepository* _repoSpeed = nullptr,
            OSDCMSInputMode* cmsOSDInputMode = nullptr,
            OSDStreamGyro* _streamInertia = nullptr,
            OSDStreamSpeed* _streamSpeed = nullptr,
            TrackWeaponEngageService *cmsEngageService = nullptr,
            EngagementDataCorrection40mmService *cmsEngageCorrService = nullptr,
            WeaponAssignmentRepository *repoWA = nullptr,
            WeaponTrackAssignmentRepository *repoWTA = nullptr
            );

private slots:
    void onTrackAssignmentResponse(BaseResponse<TrackAssignResponse> resp, bool assign);

private:
    static WeaponTrackAssignService *instance;

    TrackWeaponAssignConfig *_cmsConfig = nullptr;
    TrackWeaponEngageService *_cmsEngageService;
    EngagementDataCorrection40mmService *_cmsEngageCorrService;
    GunCoverageRepository* _repoGunCov;
    TrackBaseRepository* _repoTrack;
    OSDCMSInputMode* _cmsOSDInputMode;
    OSDInertiaRepository* _repoInertia;
    OSDSpeedRepository* _repoSpeed;
    OSDStreamGyro* _streamInertia;
    OSDStreamSpeed* _streamSpeed;
    WeaponAssignmentRepository *_repoWA;
    WeaponTrackAssignmentRepository *_repoWTA;

    bool isEngageable(const int trackId);
    bool isIdentityEngageable(const int trackId);
    bool isHeadingSpeedValid();
};

#endif // WEAPONTRACKASSIGNSERVICE_H
