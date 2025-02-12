#ifndef GUNMANAGERSERVICE_H
#define GUNMANAGERSERVICE_H

#include "src/domain/gun/repository/gun_feedback_repository.h"
#include "src/shared/config/gun_cms_config.h"
#include "src/infra/core/gun/cms/barrel/gun_cms_command_barrel.h"
#include "src/infra/core/gun/cms/status/gun_cms_command_status.h"
#include "src/usecase/gun/gun_barrel_control_mode_service.h"
#include <QObject>

class GunManagerService : public QObject
{
    Q_OBJECT
public:
    enum TECHNICAL_STATUS
    {
        OFFLINE = 0,
        ONLINE
    };
    enum OPERATIONAL_STATUS
    {
        NOT_AVAIL = 0,
        STANDBY,
        ASSIGNED
    };

    GunManagerService(GunManagerService &other) = delete;
    void operator=(const GunManagerService &) = delete;
    static GunManagerService *getInstance(
        QObject *parent = nullptr,
        GunCmsConfig *cmsConfig = nullptr,
        GunFeedbackRepository *feedbackRepo = nullptr,
        GunCommandRepository *cmdRepo = nullptr,
        GunBarrelControlModeService *modeService = nullptr
        );

    OPERATIONAL_STATUS getCurrentOpStat() const;
    TECHNICAL_STATUS getCurrentTechStat() const;

    void updateOpStatus();
    void setTechStatus(TECHNICAL_STATUS status);
    void setBarrelMode(GunBarrelModeEntity::MODE mode);
    GunBarrelModeEntity::MODE getBarrelMode() const;
    const GunBarrelEntity *getCurrentBarrel() const;
    void setBarrel(float az, float el);
    void resetBarrel(bool latestConfirm);
    void resetOpStatus();
    const GunStatusCommandEntity* getCurrentStatus() const;
    void resetStatus();
    void setStatusMount(bool on);
    void setStatusSingleShot(bool on);
    bool getStatusSingleShot() const;
    void setStatusFire(bool on);
    void setStatusProxFuze(bool on);
    void setStatusSiren(bool on);

signals:
    void OnBarrelModeResponse(GunModeBarrelResponse response, bool needConfirm);
    void OnBarrelPositionResponse(GunCommandBarrelResponse response, bool needConfirm);
    void OnStatusResponse(GunCommandStatusResponse response, bool needConfirm);
    void OnBarrelModeCheck();

private slots:
    void updateGunCommandStatus(BaseResponse<GunCommandStatusResponse> resp, bool needConfirm);
    void updateGunCommandBarrel(BaseResponse<GunCommandBarrelResponse> resp, bool needConfirm);

protected:
    GunManagerService(
        QObject *parent = nullptr,
        GunCmsConfig *cmsConfig = nullptr,
        GunFeedbackRepository *feedbackRepo = nullptr,
        GunCommandRepository *cmdRepo = nullptr,
        GunBarrelControlModeService *modeService = nullptr,
        GunCmsCommandBarrel *barrelService = nullptr,
        GunCmsCommandStatus *statusCms = nullptr
        );

private:
    static GunManagerService *gunManagerService;

    GunCmsConfig *_cmsConfig;
    GunBarrelControlModeService *_modeService;
    GunCmsCommandBarrel *_barrelService;
    GunFeedbackRepository *_feedbackRepository;
    GunCmsCommandStatus *_statusCms;
    GunCommandRepository *_repoGunCmd;

    OPERATIONAL_STATUS currentOpStat;
    TECHNICAL_STATUS currentTechStat;
};

#endif // GUNMANAGERSERVICE_H
