#ifndef WEAPONTRACKASSIGNSERVICE_H
#define WEAPONTRACKASSIGNSERVICE_H

#include "src/domain/engagement/repository/weapon_track_assign_repository.h"
#include "src/domain/weapon_assign/repository/weapon_assignment_repository.h"
#include "src/infra/http/http_client_wrapper.h"
#include <QObject>

class WeaponTrackAssignService : public QObject
{
    Q_OBJECT
public:
    WeaponTrackAssignService(WeaponTrackAssignService &other) = delete;
    void operator=(const WeaponTrackAssignService&) = delete;
    static WeaponTrackAssignService* getInstance(
            HttpClientWrapper *httpClient,
            // TODO: add track engage cms config
//            TrackEngageCmsConfig *cmsConfig = nullptr,
            WeaponAssignmentRepository *repoWA = nullptr,
            WeaponTrackAssignmentRepository *repoWTA = nullptr
            );

    // TODO: implementation
    bool SetEngagement(const QString &weapon, const int &trackId);
    bool BreakEngagement(const QString &weapon, const int &trackId);
    const WeaponTrackAssignEntity* GetEngagementTrack(const QString &weapon) const;
    QList<WeaponTrackAssignEntity*> GetEngagementWeapons(const int &trackId) const;
    QList<WeaponTrackAssignEntity*> GetAllEngagement() const;

signals:
//    void signal_setTrackAssignResponse(BaseResponse<TrackAssignModel> response);

protected:
    WeaponTrackAssignService(
            HttpClientWrapper *parent = nullptr,
            WeaponAssignmentRepository *repoWA = nullptr,
            WeaponTrackAssignmentRepository *repoWTA = nullptr
            );

private slots:
    void onReplyFinished();

private:
    static WeaponTrackAssignService *instance;

    WeaponAssignmentRepository *_repoWA;
    WeaponTrackAssignmentRepository *_repoWTA;

    //    BaseResponse<TrackAssignModel> toResponse(QByteArray raw) override;
    //    BaseResponse<TrackAssignModel> errorResponse(QNetworkReply::NetworkError err) override;
};

#endif // WEAPONTRACKASSIGNSERVICE_H
