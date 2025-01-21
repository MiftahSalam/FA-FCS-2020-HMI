#ifndef OSDCMSGYRODATA_H
#define OSDCMSGYRODATA_H

#include "src/domain/osd/repository/osd_inertia_repository.h"
#include "src/infra/http/http_client_wrapper.h"
#include "src/model/osd/cms/osd_set_gyro_request.h"
#include "src/model/base_response.h"
#include "src/model/osd/gyro_model.h"
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
            OSDCmsConfig *cmsConfig,
            OSDInertiaRepository* repoInertia = nullptr
            );

    void set(OSDSetGyroRequest request) override;

signals:
    void signal_setGyroResponse(BaseResponse<GyroModel> response);

protected:
    OSDCMSGyroData(
            HttpClientWrapper *parent = nullptr,
            OSDCmsConfig *cmsConfig = nullptr,
            OSDInertiaRepository* repoInertia = nullptr
            );

private slots:
    void onReplyFinished() override;

private:
    static OSDCMSGyroData *gyroData;
    OSDCmsConfig *cfgCms;
    OSDInertiaRepository* repoInertia;

    BaseResponse<GyroModel> toResponse(QByteArray raw) override;
    BaseResponse<GyroModel> errorResponse(QNetworkReply::NetworkError err) override;
};

#endif // OSDCMSGYRODATA_H
