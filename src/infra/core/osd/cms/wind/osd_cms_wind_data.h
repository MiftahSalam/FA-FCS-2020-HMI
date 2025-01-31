#ifndef OSDCMSWINDDATA_H
#define OSDCMSWINDDATA_H

#include "src/infra/http/http_client_wrapper.h"
#include "src/infra/core/osd/cms/wind/osd_set_wind_request.h"
#include "src/infra/core/base_response.h"
#include "src/infra/core/osd/model/wind/wind_model.h"
#include "src/shared/config/osd_cms_config.h"
#include "src/infra/http/i_cms_http.h"

class OSDCMSWindData : public HttpClientWrapper, public ICmsHttp<WindModel, OSDSetWindRequest>
{
    Q_OBJECT
public:
    OSDCMSWindData(OSDCMSWindData &other) = delete;
    void operator=(const OSDCMSWindData&) = delete;
    static OSDCMSWindData* getInstance(
        HttpClientWrapper *httpClient,
        OSDCmsConfig *cmsConfig);

    void set(OSDSetWindRequest request) override;

signals:
    void signal_setWindResponse(BaseResponse<WindModel> response);

protected:
    OSDCMSWindData(
            HttpClientWrapper *parent = nullptr,
            OSDCmsConfig *cmsConfig = nullptr
            );

private slots:
    void onReplyFinished() override;

private:
    static OSDCMSWindData *windData;
    OSDCmsConfig *cfgCms;

    BaseResponse<WindModel> toResponse(QNetworkReply::NetworkError err, QByteArray raw) override;
};

#endif // OSDCMSWINDDATA_H
