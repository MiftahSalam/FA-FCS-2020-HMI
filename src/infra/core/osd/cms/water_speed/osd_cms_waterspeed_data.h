#ifndef OSDCMSWATERSPEEDDATA_H
#define OSDCMSWATERSPEEDDATA_H

#include "src/infra/http/http_client_wrapper.h"
#include "src/infra/core/osd/cms/water_speed/osd_set_waterspeed_request.h"
#include "src/infra/core/base_response.h"
#include "src/infra/core/osd/model/water_speed/waterspeed_model.h"
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
            OSDCmsConfig *cmsConfig
            );

    void set(OSDSetWaterSpeedRequest request) override;

signals:
    void signal_setWaterSpeedResponse(BaseResponse<WaterSpeedModel> response);

protected:
    OSDCMSWaterSpeedData(
            HttpClientWrapper *parent = nullptr,
            OSDCmsConfig *cmsConfig = nullptr
            );

private slots:
    void onReplyFinished() override;

private:
    static OSDCMSWaterSpeedData *waterspeedData;
    OSDCmsConfig *cfgCms;

    BaseResponse<WaterSpeedModel> toResponse(QNetworkReply::NetworkError err, QByteArray raw) override;
};

#endif // OSDCMSWATERSPEEDDATA_H
