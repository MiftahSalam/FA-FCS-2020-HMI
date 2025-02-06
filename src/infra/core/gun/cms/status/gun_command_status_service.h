#ifndef GUNCOMMANDSTATUSSERVICE_H
#define GUNCOMMANDSTATUSSERVICE_H

#include "src/domain/gun/repository/gun_command_repository.h"
#include "src/infra/http/http_client_wrapper.h"
#include "src/infra/core/base_response.h"
#include "src/infra/core/gun/cms/status/gun_command_status_request.h"
#include "src/infra/core/gun/cms/status/gun_command_status_response.h"
#include "src/shared/config/gun_cms_config.h"
#include <QObject>

class GunCommandStatusService : public HttpClientWrapper
{
    Q_OBJECT
public:
    GunCommandStatusService(GunCommandStatusService &other) = delete;
    void operator=(const GunCommandStatusService&) = delete;
    static GunCommandStatusService* getInstance(
            HttpClientWrapper *httpClient,
            GunCmsConfig *cmsConfig = nullptr,
            GunCommandRepository *repoGunCmd = nullptr
            );

    const GunStatusCommandEntity* getCurrentStatus() const;
    void setStatus(GunCommandStatusRequest request);
    void setStatusMount(bool on);
    void setStatusSingleShot(bool on);
    void setStatusFire(bool on);
    void setStatusProxFuze(bool on);
    void setStatusSiren(bool on);

signals:
    void signal_setStatusResponse(BaseResponse<GunCommandStatusResponse> response);

protected:
    GunCommandStatusService(
            HttpClientWrapper *parent = nullptr,
            GunCmsConfig *cmsConfig = nullptr,
            GunCommandRepository *repoGunCmd = nullptr
            );

private slots:
    void onReplyFinished() override;

private:
    static GunCommandStatusService *instance;
    GunCmsConfig *cfgCms;
    GunCommandRepository* _repoGunCmd;

    BaseResponse<GunCommandStatusResponse> toResponse(QByteArray raw);
    BaseResponse<GunCommandStatusResponse> errorResponse(QNetworkReply::NetworkError err);
    void sendStatus(GunCommandStatusRequest request);
};

#endif // GUNCOMMANDSTATUSSERVICE_H
