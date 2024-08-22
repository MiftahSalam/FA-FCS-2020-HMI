#ifndef GUNCOMMANDBARRELMODESERVICE_H
#define GUNCOMMANDBARRELMODESERVICE_H

#include "src/domain/gun/repository/gun_command_repository.h"
#include "src/infra/http/http_client_wrapper.h"
#include "src/model/base_response.h"
#include "src/model/gun/cms/gun_mode_barrel_request.h"
#include "src/model/gun/cms/gun_mode_barrel_response.h"
#include "src/shared/config/gun_cms_config.h"
#include <QObject>

class GunCommandBarrelModeService : public HttpClientWrapper
{
    Q_OBJECT
public:
    GunCommandBarrelModeService(GunCommandBarrelModeService &other) = delete;
    void operator=(const GunCommandBarrelModeService&) = delete;
    static GunCommandBarrelModeService* getInstance(
            HttpClientWrapper *httpClient,
            GunCmsConfig *cmsConfig = nullptr,
            GunCommandRepository *repoGunCmd = nullptr
            );

    void setMode(GunModeBarrelRequest request);

signals:
    void signal_setModeResponse(BaseResponse<GunModeBarrelResponse> response);

protected:
    GunCommandBarrelModeService(
            HttpClientWrapper *parent = nullptr,
            GunCmsConfig *cmsConfig = nullptr,
            GunCommandRepository *repoGunCmd = nullptr
            );

private slots:
    void onReplyFinished() override;

private:
    static GunCommandBarrelModeService *instance;
    GunCmsConfig *cfgCms;
    GunCommandRepository* _repoGunCmd;

    BaseResponse<GunModeBarrelResponse> toResponse(QByteArray raw);
    BaseResponse<GunModeBarrelResponse> errorResponse(QNetworkReply::NetworkError err);
};

#endif // GUNCOMMANDBARRELMODESERVICE_H
