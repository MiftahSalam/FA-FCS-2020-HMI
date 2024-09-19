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
    void sendResetAssignment(const QString weapon);

signals:
    void signal_trackAssignmentResponse(BaseResponse<TrackAssignResponse> response, bool assign);

protected:
    TrackWeaponEngageService(
            HttpClientWrapper *parent = nullptr,
            TrackWeaponAssignConfig *cmsConfig = nullptr
            );

private slots:
    void onReplyFinished() override;
    void onResetAssignReplyFinished();

private:
    static TrackWeaponEngageService *instance;

    TrackWeaponAssignConfig *_cmsConfig;

    BaseResponse<TrackAssignResponse> toResponse(QByteArray raw);
    BaseResponse<TrackAssignResponse> errorHttpResponse(QNetworkReply::NetworkError err);

    bool isAssignResponse();
};

#endif // TRACKWEAPONENGAGESERVICE_H
