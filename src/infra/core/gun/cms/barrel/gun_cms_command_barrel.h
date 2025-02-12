#ifndef GunCommandBarrelService_H
#define GunCommandBarrelService_H

#include "src/domain/gun/repository/gun_command_repository.h"
#include "src/infra/http/http_client_wrapper.h"
#include "src/infra/core/base_response.h"
#include "src/infra/core/gun/cms/barrel/gun_command_barrel_request.h"
#include "src/infra/core/gun/cms/barrel/gun_command_barrel_response.h"
#include "src/infra/http/i_cms_http.h"
#include "src/shared/config/gun_cms_config.h"
#include <QObject>

class GunCmsCommandBarrel : public HttpClientWrapper, public ICmsHttp<GunCommandBarrelResponse, GunCommandBarrelRequest>
{
    Q_OBJECT
public:
    GunCmsCommandBarrel(GunCmsCommandBarrel &other) = delete;
    void operator=(const GunCmsCommandBarrel&) = delete;
    static GunCmsCommandBarrel* getInstance(
            HttpClientWrapper *httpClient,
            GunCmsConfig *cmsConfig = nullptr
            );

    void setBarrelWithConfirm(GunCommandBarrelRequest request, bool confirm);

signals:
    void signal_setBarrelResponse(BaseResponse<GunCommandBarrelResponse> response, bool confirm);

protected:
    GunCmsCommandBarrel(
            HttpClientWrapper *parent = nullptr,
            GunCmsConfig *cmsConfig = nullptr
            );

private slots:
    void onReplyFinished() override;

private:
    static GunCmsCommandBarrel *instance;
    GunCmsConfig *cfgCms;

    BaseResponse<GunCommandBarrelResponse> toResponse(QNetworkReply::NetworkError err, QByteArray raw) override;
    void set(GunCommandBarrelRequest request) override;
};

#endif // GunCommandBarrelService_H
