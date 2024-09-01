#ifndef TRACKWEAPONENGAGESERVICE_H
#define TRACKWEAPONENGAGESERVICE_H

#include "src/infra/http/http_client_wrapper.h"
#include "src/model/base_response.h"
#include "src/model/engagement/cms/track_assign_request.h"
#include "src/model/engagement/cms/track_assign_response.h"
#include "src/shared/config/track_weapon_assign_config.h"
#include <QObject>

class TrackWeaponEngageService : public HttpClientWrapper
{
    Q_OBJECT
public:
    TrackWeaponEngageService(TrackWeaponEngageService &other) = delete;
    void operator=(const TrackWeaponEngageService&) = delete;
    static TrackWeaponEngageService* getInstance(
            HttpClientWrapper *httpClient,
            TrackWeaponAssignConfig *cmsConfig = nullptr
            );

    void sendAssignment(TrackAssignRequest request, bool assign);

signals:
//    void signal_setCorrectionResponse(BaseResponse<EngagementCorrectionSetRequest> response);
    void signal_trackAssignmentResponse(BaseResponse<TrackAssignResponse> response, bool assign);

protected:
    TrackWeaponEngageService(
            HttpClientWrapper *parent = nullptr,
            // TODO: add engagement correction cms config
            TrackWeaponAssignConfig *cmsConfig = nullptr
//            EngagementDataCorrectionRepository *repoEngCorr = nullptr
            );

private slots:
    void onReplyFinished() override;

private:
    static TrackWeaponEngageService *instance;

    TrackWeaponAssignConfig *_cmsConfig;

    // TODO: add engagement correction cms config
//    EngagementDataCorrectionRepository* _repoEngCorr;

    BaseResponse<TrackAssignResponse> toResponse(QByteArray raw);
    BaseResponse<TrackAssignResponse> errorResponse(QNetworkReply::NetworkError err);
};

#endif // TRACKWEAPONENGAGESERVICE_H
