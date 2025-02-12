#ifndef GunCommandBarrelService_H
#define GunCommandBarrelService_H

#include "src/domain/gun/repository/gun_command_repository.h"
#include "src/infra/http/http_client_wrapper.h"
#include "src/infra/core/base_response.h"
#include "src/infra/core/gun/cms/barrel/gun_command_barrel_request.h"
#include "src/infra/core/gun/cms/barrel/gun_command_barrel_response.h"
#include "src/shared/config/gun_cms_config.h"
#include <QObject>

class GunCmsCommandBarrel : public HttpClientWrapper
{
    Q_OBJECT
public:
    GunCmsCommandBarrel(GunCmsCommandBarrel &other) = delete;
    void operator=(const GunCmsCommandBarrel&) = delete;
    static GunCmsCommandBarrel* getInstance(
            HttpClientWrapper *httpClient,
            GunCmsConfig *cmsConfig = nullptr,
            GunCommandRepository *repoGunCmd = nullptr
            );

    void setBarrelWithConfirm(GunCommandBarrelRequest request, bool confirm);

signals:
    void signal_setBarrelResponse(BaseResponse<GunCommandBarrelResponse> response, bool confirm);

protected:
    GunCmsCommandBarrel(
            HttpClientWrapper *parent = nullptr,
            GunCmsConfig *cmsConfig = nullptr,
            GunCommandRepository *repoGunCmd = nullptr
            );

private slots:
    void onReplyFinished() override;

private:
    static GunCmsCommandBarrel *instance;
    GunCmsConfig *cfgCms;
    GunCommandRepository* _repoGunCmd;
    bool latestConfirm;

    BaseResponse<GunCommandBarrelResponse> toResponse(QByteArray raw);
    BaseResponse<GunCommandBarrelResponse> errorResponse(QNetworkReply::NetworkError err);
};

#endif // GunCommandBarrelService_H
