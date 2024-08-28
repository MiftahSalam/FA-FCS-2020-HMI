#ifndef GUNMANAGERSERVICE_H
#define GUNMANAGERSERVICE_H

#include "src/domain/gun/repository/gun_feedback_repository.h"
#include "src/shared/config/gun_cms_config.h"
#include "src/usecase/gun/cms/gun_command_barrel_mode_service.h"
#include "src/usecase/gun/cms/gun_command_barrel_service.h"
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
        GunCommandRepository *cmdRepo = nullptr);

    OPERATIONAL_STATUS getCurrentOpStat() const;
    TECHNICAL_STATUS getCurrentTechStat() const;

    void updateOpStatus();
    void setTechStatus(TECHNICAL_STATUS status);
    void setBarrelMode(GunBarrelModeEntity::MODE mode);
    GunBarrelModeEntity::MODE getBarrelMode() const;
    const GunBarrelEntity *getCurrentBarrel() const;
    void setBarrel(float az, float el);
    void resetBarrel();
    void resetOpStatus();

signals:
    void OnBarrelModeResponse(BaseResponse<GunModeBarrelResponse> response, bool needConfirm);
    void OnBarrelPositionResponse(BaseResponse<GunCommandBarrelResponse> response);
    void OnBarrelModeCheck();

protected:
    GunManagerService(
        QObject *parent = nullptr,
        GunCmsConfig *cmsConfig = nullptr,
        GunFeedbackRepository *feedbackRepo = nullptr,
        GunCommandBarrelModeService *modeService = nullptr,
        GunCommandBarrelService *barrelService = nullptr
        // TODO : injct gun command status service
        //            GunCommandStatus *statusService= nullptr,
    );

private:
    static GunManagerService *gunManagerService;

    GunCmsConfig *_cmsConfig;
    GunCommandBarrelModeService *_modeService;
    GunCommandBarrelService *_barrelService;
    GunFeedbackRepository *_feedbackRepository;

    // TODO : injct gun command status service
    //            GunCommandStatus *statusService= nullptr,
    OPERATIONAL_STATUS currentOpStat;
    TECHNICAL_STATUS currentTechStat;
};

#endif // GUNMANAGERSERVICE_H
