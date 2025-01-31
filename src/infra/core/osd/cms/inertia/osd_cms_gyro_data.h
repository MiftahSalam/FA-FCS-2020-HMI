#ifndef OSDCMSGYRODATA_H
#define OSDCMSGYRODATA_H

#include "src/infra/http/http_client_wrapper.h"
#include "src/infra/core/osd/cms/inertia/osd_set_gyro_request.h"
#include "src/infra/core/base_response.h"
#include "src/infra/core/osd/model/inertia/gyro_model.h"
#include "src/shared/config/osd_cms_config.h"
#include "src/infra/http/i_cms_http.h"

class OSDCMSGyroData : public HttpClientWrapper, public ICmsHttp<GyroModel, OSDSetGyroRequest>
{
    Q_OBJECT
public:
    OSDCMSGyroData(OSDCMSGyroData &other) = delete;
    void operator=(const OSDCMSGyroData&) = delete;
    static OSDCMSGyroData* getInstance(
            HttpClientWrapper *httpClient,
            OSDCmsConfig *cmsConfig
            );

    void set(OSDSetGyroRequest request) override;

signals:
    void signal_setGyroResponse(BaseResponse<GyroModel> response);

protected:
    OSDCMSGyroData(
            HttpClientWrapper *parent = nullptr,
            OSDCmsConfig *cmsConfig = nullptr
            );

private slots:
    void onReplyFinished() override;

private:
    static OSDCMSGyroData *gyroData;
    OSDCmsConfig *cfgCms;

    BaseResponse<GyroModel> toResponse(QNetworkReply::NetworkError err, QByteArray raw) override;
};

#endif // OSDCMSGYRODATA_H
