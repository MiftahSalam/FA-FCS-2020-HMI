#ifndef OSDCMS_H
#define OSDCMS_H

#include <QNetworkReply>
#include <QObject>

#include "src/infra/http/http_client_wrapper.h"
#include "src/model/osd/cms/osd_set_position_request.h"
#include "src/model/base_response.h"
#include "src/model/osd/position_model.h"
#include "src/shared/config/osd_cms_config.h"

class OSDCMSPositionData;

class OSDCMS : public QObject
{
    Q_OBJECT
public:
    explicit OSDCMS(QObject *parent = nullptr, OSDCmsConfig *cmsConfig = nullptr);

    OSDCMSPositionData *getServiceOSDCMSPosition() const;

private:
    OSDCmsConfig *cfgCms;
    OSDCMSPositionData *serviceOSDCMSPosition;
};


class OSDCMSPositionData : public HttpClientWrapper
{
    Q_OBJECT
public:
    explicit OSDCMSPositionData(HttpClientWrapper *parent = nullptr, OSDCmsConfig *cmsConfig = nullptr);

    void setPosition(OSDSetPositionRequest request);

signals:
    void signal_setPositionResponse(BaseResponse<PositionModel> response);

private slots:
    void onReplyFinished() override;

private:
    OSDCmsConfig *cfgCms;

    BaseResponse<PositionModel> toResponse(QByteArray raw);
    BaseResponse<PositionModel> errorResponse(QNetworkReply::NetworkError err);
};

#endif // OSDCMS_H
