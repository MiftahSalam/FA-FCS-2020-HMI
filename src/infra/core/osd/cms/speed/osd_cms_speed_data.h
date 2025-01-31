#ifndef OSDCMSSPEEDDATA_H
#define OSDCMSSPEEDDATA_H

#include "src/infra/http/http_client_wrapper.h"
#include "src/infra/core/osd/cms/speed/osd_set_speed_request.h"
#include "src/infra/core/base_response.h"
#include "src/infra/core/osd/model/speed/speed_model.h"
#include "src/shared/config/osd_cms_config.h"
#include "src/infra/http/i_cms_http.h"

class OSDCMSSpeedData: public HttpClientWrapper, public ICmsHttp<SpeedModel, OSDSetSpeedRequest>
{
    Q_OBJECT
public:
    OSDCMSSpeedData(OSDCMSSpeedData &other) = delete;
    void operator=(const OSDCMSSpeedData&) = delete;
    static OSDCMSSpeedData* getInstance(
            HttpClientWrapper *httpClient,
            OSDCmsConfig *cmsConfig
            );

    void set(OSDSetSpeedRequest request) override;

signals:
    void signal_setSpeedResponse(BaseResponse<SpeedModel> response);

protected:
    OSDCMSSpeedData(
            HttpClientWrapper *parent = nullptr,
            OSDCmsConfig *cmsConfig = nullptr
            );

private slots:
    void onReplyFinished() override;

private:
    static OSDCMSSpeedData *speedData;
    OSDCmsConfig *cfgCms;

    BaseResponse<SpeedModel> toResponse(QNetworkReply::NetworkError err, QByteArray raw) override;
};

#endif // OSDCMSSPEEDDATA_H
