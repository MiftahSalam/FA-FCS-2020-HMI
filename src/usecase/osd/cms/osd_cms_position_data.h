#ifndef OSDCMSPOSITIONDATA_H
#define OSDCMSPOSITIONDATA_H

#include "src/domain/osd/repository/osd_position_repository.h"
#include "src/infra/http/http_client_wrapper.h"
#include "src/model/osd/cms/osd_set_position_request.h"
#include "src/model/base_response.h"
#include "src/model/osd/position_model.h"
#include "src/shared/config/osd_cms_config.h"
#include "src/usecase/osd/cms/i_osd_cms.h"

class OSDCMSPositionData : public HttpClientWrapper, public IOSDCMS<PositionModel, OSDSetPositionRequest>
{
    Q_OBJECT
public:
    OSDCMSPositionData(OSDCMSPositionData &other) = delete;
    void operator=(const OSDCMSPositionData&) = delete;
    static OSDCMSPositionData* getInstance(
            HttpClientWrapper *httpClient,
            OSDCmsConfig *cmsConfig,
            OSDPositionRepository* repoPos = nullptr
            );

    void set(OSDSetPositionRequest request) override;

signals:
    void signal_setPositionResponse(BaseResponse<PositionModel> response);

protected:
    OSDCMSPositionData(
            HttpClientWrapper *parent = nullptr,
            OSDCmsConfig *cmsConfig = nullptr,
            OSDPositionRepository* repoPos = nullptr
            );

private slots:
    void onReplyFinished() override;

private:
    static OSDCMSPositionData *positionData;
    OSDCmsConfig *cfgCms;
    OSDPositionRepository* repoPos;

    BaseResponse<PositionModel> toResponse(QByteArray raw) override;
    BaseResponse<PositionModel> errorResponse(QNetworkReply::NetworkError err) override;
};
#endif // OSDCMSPOSITIONDATA_H
