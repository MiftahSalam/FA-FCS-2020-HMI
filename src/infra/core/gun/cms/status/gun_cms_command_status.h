#ifndef GUNCOMMANDSTATUSSERVICE_H
#define GUNCOMMANDSTATUSSERVICE_H

#include <QObject>

#include "src/infra/http/http_client_wrapper.h"
#include "src/infra/core/base_response.h"
#include "src/infra/core/gun/cms/status/gun_command_status_request.h"
#include "src/infra/core/gun/cms/status/gun_command_status_response.h"
#include "src/infra/http/i_cms_http.h"
#include "src/shared/config/gun_cms_config.h"

class GunCmsCommandStatus : public HttpClientWrapper, public ICmsHttp<GunCommandStatusResponse, GunCommandStatusRequest>
{
    Q_OBJECT
public:
    GunCmsCommandStatus(GunCmsCommandStatus &other) = delete;
    void operator=(const GunCmsCommandStatus&) = delete;
    static GunCmsCommandStatus* getInstance(
            HttpClientWrapper *httpClient,
            GunCmsConfig *cmsConfig = nullptr
            );

    void setStatus(GunCommandStatusRequest request);

signals:
    void signal_setStatusResponse(BaseResponse<GunCommandStatusResponse> response, bool needConfirm);

protected:
    GunCmsCommandStatus(
            HttpClientWrapper *parent = nullptr,
            GunCmsConfig *cmsConfig = nullptr
            );

private slots:
    void onReplyFinished() override;

private:
    static GunCmsCommandStatus *instance;
    GunCmsConfig *cfgCms;

    BaseResponse<GunCommandStatusResponse> toResponse(QNetworkReply::NetworkError err, QByteArray raw) override;
    void set(GunCommandStatusRequest request) override;
};

#endif // GUNCOMMANDSTATUSSERVICE_H
