#ifndef ENGAGEMENTDATACORRECTION40MMSERVICE_H
#define ENGAGEMENTDATACORRECTION40MMSERVICE_H

#include "src/infra/http/http_client_wrapper.h"
#include "src/model/base_response.h"
#include "src/model/engagement/cms/engagement_correction_set_request.h"
#include "src/model/engagement/cms/engagement_correction_set_response.h"
#include "src/shared/config/track_weapon_assign_config.h"
#include <QObject>

class EngagementDataCorrection40mmService : public HttpClientWrapper
{
    Q_OBJECT
public:
    EngagementDataCorrection40mmService(EngagementDataCorrection40mmService &other) = delete;
    void operator=(const EngagementDataCorrection40mmService&) = delete;
    static EngagementDataCorrection40mmService* getInstance(
            HttpClientWrapper *httpClient,
            TrackWeaponAssignConfig *cmsConfig = nullptr
            );

    // TODO: add request methods implementation
   void setCorrection(EngagementCorrectionSetRequest request);

signals:
   void signal_setCorrectionResponse(BaseResponse<EngagementCorrectionSetResponse> response);

protected:
    EngagementDataCorrection40mmService(
            HttpClientWrapper *parent = nullptr,
            TrackWeaponAssignConfig *cmsConfig = nullptr
            );

private slots:
    void onReplyFinished();

private:
    static EngagementDataCorrection40mmService *instance;
    TrackWeaponAssignConfig *cfgCms;

    BaseResponse<EngagementCorrectionSetResponse> toResponse(QByteArray raw);
    BaseResponse<EngagementCorrectionSetResponse> errorResponse(QNetworkReply::NetworkError err);
};

#endif // ENGAGEMENTDATACORRECTION40MMSERVICE_H
