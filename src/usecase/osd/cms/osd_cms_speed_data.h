#ifndef OSDCMSSPEEDDATA_H
#define OSDCMSSPEEDDATA_H

#include "src/domain/osd/repository/osd_speed_repository.h"
#include "src/infra/http/http_client_wrapper.h"
#include "src/model/osd/cms/osd_set_speed_request.h"
#include "src/model/base_response.h"
#include "src/model/osd/speed_model.h"
#include "src/shared/config/osd_cms_config.h"
#include "src/usecase/osd/cms/i_cms_http.h"

class OSDCMSSpeedData: public HttpClientWrapper, public ICmsHttp<SpeedModel, OSDSetSpeedRequest>
{
    Q_OBJECT
public:
    OSDCMSSpeedData(OSDCMSSpeedData &other) = delete;
    void operator=(const OSDCMSSpeedData&) = delete;
    static OSDCMSSpeedData* getInstance(
            HttpClientWrapper *httpClient,
            OSDCmsConfig *cmsConfig,
            OSDSpeedRepository* repoSpeed = nullptr
            );

    void set(OSDSetSpeedRequest request) override;

signals:
    void signal_setSpeedResponse(BaseResponse<SpeedModel> response);

protected:
    OSDCMSSpeedData(
            HttpClientWrapper *parent = nullptr,
            OSDCmsConfig *cmsConfig = nullptr,
            OSDSpeedRepository* repoSpeed = nullptr
            );

private slots:
    void onReplyFinished() override;

private:
    static OSDCMSSpeedData *speedData;
    OSDCmsConfig *cfgCms;
    OSDSpeedRepository* repoSpeed;

    BaseResponse<SpeedModel> toResponse(QByteArray raw) override;
    BaseResponse<SpeedModel> errorResponse(QNetworkReply::NetworkError err) override;

};

#endif // OSDCMSSPEEDDATA_H
