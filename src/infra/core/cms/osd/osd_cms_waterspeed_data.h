#ifndef OSDCMSWATERSPEEDDATA_H
#define OSDCMSWATERSPEEDDATA_H

#include "src/domain/osd/repository/osd_waterspeed_repository.h"
#include "src/infra/http/http_client_wrapper.h"
#include "src/model/osd/cms/osd_set_waterspeed_request.h"
#include "src/model/base_response.h"
#include "src/model/osd/waterspeed_model.h"
#include "src/shared/config/osd_cms_config.h"
#include "src/infra/http/i_cms_http.h"

class OSDCMSWaterSpeedData : public HttpClientWrapper, public ICmsHttp<WaterSpeedModel, OSDSetWaterSpeedRequest>
{
    Q_OBJECT
public:
    OSDCMSWaterSpeedData(OSDCMSWaterSpeedData &other) = delete;
    void operator=(const OSDCMSWaterSpeedData&) = delete;
    static OSDCMSWaterSpeedData* getInstance(
            HttpClientWrapper *httpClient,
            OSDCmsConfig *cmsConfig,
            OSDWaterSpeedRepository* repoWS = nullptr
            );

    void set(OSDSetWaterSpeedRequest request) override;

signals:
    void signal_setWaterSpeedResponse(BaseResponse<WaterSpeedModel> response);

protected:
    OSDCMSWaterSpeedData(
            HttpClientWrapper *parent = nullptr,
            OSDCmsConfig *cmsConfig = nullptr,
            OSDWaterSpeedRepository* repoWS = nullptr
            );

private slots:
    void onReplyFinished() override;

private:
    static OSDCMSWaterSpeedData *waterspeedData;
    OSDCmsConfig *cfgCms;
    OSDWaterSpeedRepository* repoWS;

    BaseResponse<WaterSpeedModel> toResponse(QByteArray raw) override;
    BaseResponse<WaterSpeedModel> errorResponse(QNetworkReply::NetworkError err) override;
};

#endif // OSDCMSWATERSPEEDDATA_H
