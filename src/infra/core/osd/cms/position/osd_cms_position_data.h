#ifndef OSDCMSPOSITIONDATA_H
#define OSDCMSPOSITIONDATA_H

#include "src/infra/http/http_client_wrapper.h"
#include "src/infra/core/osd/cms/position/osd_set_position_request.h"
#include "src/infra/core/base_response.h"
#include "src/infra/core/osd/model/position/position_cms_response_model.h"
#include "src/shared/config/osd_cms_config.h"
#include "src/infra/http/i_cms_http.h"

class OSDCMSPositionData : public HttpClientWrapper, public ICmsHttp<PositionResponseModel, OSDSetPositionRequest>
{
    Q_OBJECT
public:
    OSDCMSPositionData(OSDCMSPositionData &other) = delete;
    void operator=(const OSDCMSPositionData&) = delete;
    static OSDCMSPositionData* getInstance(
            HttpClientWrapper *httpClient,
            OSDCmsConfig *cmsConfig
            );

    void set(OSDSetPositionRequest request) override;

signals:
    void signal_setPositionResponse(BaseResponse<PositionResponseModel> response);

protected:
    OSDCMSPositionData(
            HttpClientWrapper *parent = nullptr,
            OSDCmsConfig *cmsConfig = nullptr
            );

private slots:
    void onReplyFinished() override;

private:
    static OSDCMSPositionData *positionData;
    OSDCmsConfig *cfgCms;

    BaseResponse<PositionResponseModel> toResponse(QNetworkReply::NetworkError err, QByteArray raw) override;
};
#endif // OSDCMSPOSITIONDATA_H
