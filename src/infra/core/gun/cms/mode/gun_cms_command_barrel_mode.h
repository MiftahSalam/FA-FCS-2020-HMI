#ifndef GunCmsCommandBarrelMode_H
#define GunCmsCommandBarrelMode_H

#include "src/infra/http/http_client_wrapper.h"
#include "src/infra/core/base_response.h"
#include "src/infra/core/gun/cms/mode/gun_mode_barrel_request.h"
#include "src/infra/core/gun/cms/mode/gun_mode_barrel_response.h"
#include "src/infra/http/i_cms_http.h"
#include "src/shared/config/gun_cms_config.h"
#include <QObject>

class GunCmsCommandBarrelMode : public HttpClientWrapper, public ICmsHttp<GunModeBarrelResponse, GunModeBarrelRequest>
{
    Q_OBJECT
public:
    GunCmsCommandBarrelMode(GunCmsCommandBarrelMode &other) = delete;
    void operator=(const GunCmsCommandBarrelMode&) = delete;
    static GunCmsCommandBarrelMode* getInstance(
            HttpClientWrapper *httpClient,
            GunCmsConfig *cmsConfig = nullptr
            );

    void sendMode(const bool confirm, GunModeBarrelRequest request);

signals:
    void signal_setModeResponse(BaseResponse<GunModeBarrelResponse> response, bool needConfirm);

protected:
    GunCmsCommandBarrelMode(
            HttpClientWrapper *parent = nullptr,
            GunCmsConfig *cmsConfig = nullptr
            );

private slots:
    void onReplyFinished() override;

private:
    static GunCmsCommandBarrelMode *instance;
    GunCmsConfig *cfgCms;

    void set(GunModeBarrelRequest request) override;
    BaseResponse<GunModeBarrelResponse> toResponse(QNetworkReply::NetworkError err, QByteArray raw) override;
};

#endif // GunCmsCommandBarrelMode_H
