#ifndef GunCommandBarrelService_H
#define GunCommandBarrelService_H

#include "src/domain/gun/repository/gun_command_repository.h"
#include "src/infra/http/http_client_wrapper.h"
#include "src/model/base_response.h"
#include "src/model/gun/cms/gun_command_barrel_request.h"
#include "src/model/gun/cms/gun_command_barrel_response.h"
#include "src/shared/config/gun_cms_config.h"
#include <QObject>

class GunCommandBarrelService : public HttpClientWrapper
{
    Q_OBJECT
public:
    GunCommandBarrelService(GunCommandBarrelService &other) = delete;
    void operator=(const GunCommandBarrelService&) = delete;
    static GunCommandBarrelService* getInstance(
            HttpClientWrapper *httpClient,
            GunCmsConfig *cmsConfig = nullptr,
            GunCommandRepository *repoGunCmd = nullptr
            );

    void setBarrelWithConfirm(GunCommandBarrelRequest request, bool confirm);

signals:
    void signal_setBarrelResponse(BaseResponse<GunCommandBarrelResponse> response, bool confirm);

protected:
    GunCommandBarrelService(
            HttpClientWrapper *parent = nullptr,
            GunCmsConfig *cmsConfig = nullptr,
            GunCommandRepository *repoGunCmd = nullptr
            );

private slots:
    void onReplyFinished() override;

private:
    static GunCommandBarrelService *instance;
    GunCmsConfig *cfgCms;
    GunCommandRepository* _repoGunCmd;
    bool latestConfirm;

    BaseResponse<GunCommandBarrelResponse> toResponse(QByteArray raw);
    BaseResponse<GunCommandBarrelResponse> errorResponse(QNetworkReply::NetworkError err);
};

#endif // GunCommandBarrelService_H
