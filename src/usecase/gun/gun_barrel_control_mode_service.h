#ifndef GUN_BARREL_CONTROL_MODE_SERVICE_H
#define GUN_BARREL_CONTROL_MODE_SERVICE_H

#include "qtimer.h"
#include "src/domain/gun/repository/gun_command_repository.h"
#include "src/infra/core/gun/cms/mode/gun_cms_command_barrel_mode.h"
#include <QObject>

class GunBarrelControlModeService : public QObject
{
    Q_OBJECT
public:
    GunBarrelControlModeService(GunBarrelControlModeService &other) = delete;
    void operator=(const GunBarrelControlModeService&) = delete;

    static GunBarrelControlModeService* getInstance(
        QObject *parent,
        GunCommandRepository *repoGunCmd = nullptr,
        GunCmsCommandBarrelMode* gunModeCms = nullptr
        );

    void setModeWithConfirm(GunBarrelModeEntity::MODE mode, bool confirm);
    GunBarrelModeEntity::MODE getMode() const;

signals:
    void signal_processedResponse(GunModeBarrelResponse data, bool needConfirm);
    void signal_modeCheck();

protected:
    GunBarrelControlModeService(
        QObject *parent = nullptr,
        GunCommandRepository *repoGunCmd = nullptr,
        GunCmsCommandBarrelMode* gunModeCms = nullptr
        );

private slots:
    void onTimerTimeout();

    void onUpdateBarrelMode(BaseResponse<GunModeBarrelResponse> resp, bool needConfirm);

private:
    static GunBarrelControlModeService *instance;
    GunCommandRepository* _repoGunCmd;
    GunCmsCommandBarrelMode* _cmsGunMode;

    QTimer *timer;

    GunBarrelModeEntity::MODE previousMode;
    bool synced;
    // bool requestSync;

    BaseResponse<GunModeBarrelResponse> toResponse(QByteArray raw);
    BaseResponse<GunModeBarrelResponse> errorResponse(QNetworkReply::NetworkError err);
    void sync();
    void sendMode(GunModeBarrelRequest request);
    bool gunBarrelModeEntityToBool(GunBarrelModeEntity::MODE mode);
    GunBarrelModeEntity::MODE gunResponseModeToBarrelMode(bool mode);
};

#endif // GUN_BARREL_CONTROL_MODE_SERVICE_H
