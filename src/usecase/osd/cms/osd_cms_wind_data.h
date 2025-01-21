#ifndef OSDCMSWINDDATA_H
#define OSDCMSWINDDATA_H

#include "src/domain/osd/repository/osd_wind_repository.h"
#include "src/infra/http/http_client_wrapper.h"
#include "src/model/osd/cms/osd_set_wind_request.h"
#include "src/model/base_response.h"
#include "src/model/osd/wind_model.h"
#include "src/shared/config/osd_cms_config.h"
#include "src/usecase/osd/cms/i_cms_http.h"

class OSDCMSWindData : public HttpClientWrapper, public ICmsHttp<WindModel, OSDSetWindRequest>
{
    Q_OBJECT
public:
    OSDCMSWindData(OSDCMSWindData &other) = delete;
    void operator=(const OSDCMSWindData&) = delete;
    static OSDCMSWindData* getInstance(
            HttpClientWrapper *httpClient,
            OSDCmsConfig *cmsConfig,
            OSDWindRepository* repoWind = nullptr
            );

    void set(OSDSetWindRequest request) override;

signals:
    void signal_setWindResponse(BaseResponse<WindModel> response);

protected:
    OSDCMSWindData(
            HttpClientWrapper *parent = nullptr,
            OSDCmsConfig *cmsConfig = nullptr,
            OSDWindRepository* repoWind = nullptr
            );

private slots:
    void onReplyFinished() override;

private:
    static OSDCMSWindData *windData;
    OSDCmsConfig *cfgCms;
    OSDWindRepository* repoWind;

    BaseResponse<WindModel> toResponse(QByteArray raw) override;
    BaseResponse<WindModel> errorResponse(QNetworkReply::NetworkError err) override;
};

#endif // OSDCMSWINDDATA_H
