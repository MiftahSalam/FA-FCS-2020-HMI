#ifndef GUNCOMMANDBARRELMODESERVICE_H
#define GUNCOMMANDBARRELMODESERVICE_H

#include "qtimer.h"
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

    void setModeWithConfirm(GunBarrelModeEntity::MODE mode, bool confirm);
    GunBarrelModeEntity::MODE getMode() const;

signals:
    void signal_setModeResponse(BaseResponse<GunModeBarrelResponse> response, bool needConfirm);
    void signal_modeCheck();

protected:
    GunCommandBarrelModeService(
            HttpClientWrapper *parent = nullptr,
            GunCmsConfig *cmsConfig = nullptr,
            GunCommandRepository *repoGunCmd = nullptr
            );

private slots:
    void onReplyFinished() override;

    void onTimerTimeout();

private:
    static GunCommandBarrelModeService *instance;
    GunCmsConfig *cfgCms;
    GunCommandRepository* _repoGunCmd;

    QTimer *timer;

    GunBarrelModeEntity::MODE previousMode;
    bool synced;
    bool requestSync;

    BaseResponse<GunModeBarrelResponse> toResponse(QByteArray raw);
    BaseResponse<GunModeBarrelResponse> errorResponse(QNetworkReply::NetworkError err);
    void sync();
    void sendMode(GunModeBarrelRequest request);
    bool gunBarrelModeEntityToBool(GunBarrelModeEntity::MODE mode);
    GunBarrelModeEntity::MODE gunResponseModeToBarrelMode(bool mode);
};

#endif // GUNCOMMANDBARRELMODESERVICE_H
