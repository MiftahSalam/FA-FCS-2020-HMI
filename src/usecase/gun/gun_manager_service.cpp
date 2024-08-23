#include "gun_manager_service.h"
#include "src/shared/common/errors/err_object_creation.h"

GunManagerService* GunManagerService::gunManagerService = nullptr;

GunManagerService::GunManagerService(
        QObject *parent,
        GunCmsConfig *cmsConfig,
        GunFeedbackRepository *feedbackRepo,
        GunCommandBarrelModeService *modeService,
        GunCommandBarrelService *barrelService
        ):
    QObject(parent),
    _cmsConfig(cmsConfig),
    _modeService(modeService),
    _barrelService(barrelService),
    _feedbackRepository(feedbackRepo),
    currentOpStat(GunManagerService::NOT_AVAIL),
    currentTechStat(GunManagerService::OFFLINE)
{
}

GunManagerService::TECHNICAL_STATUS GunManagerService::getCurrentTechStat() const
{
    return currentTechStat;
}

void GunManagerService::updateOpStatus()
{
    switch (currentTechStat) {
    case GunManagerService::ONLINE:
    {
        bool remote = _feedbackRepository->GetStatus()->remote();
        if (remote) {
            switch (_modeService->getMode()) {
            case GunBarrelModeEntity::NONE:
                currentOpStat = OPERATIONAL_STATUS::STANDBY;
                break;
            case GunBarrelModeEntity::MANUAL:
            case GunBarrelModeEntity::AUTO:
                currentOpStat = OPERATIONAL_STATUS::ASSIGNED;
                break;
            default:
                break;
            }
        } else {
            currentOpStat = OPERATIONAL_STATUS::NOT_AVAIL;
        }
    }
        break;
    default:
        currentOpStat = OPERATIONAL_STATUS::NOT_AVAIL;
        break;
    }
}

void GunManagerService::setTechStatus(TECHNICAL_STATUS status)
{
    currentTechStat = status;
}

void GunManagerService::setBarrelMode(GunBarrelModeEntity::MODE mode)
{
    _modeService->setMode(mode);
}

void GunManagerService::resetBarrel()
{
    _barrelService->setBarrel(GunCommandBarrelRequest(0.,0.));
}

GunBarrelModeEntity::MODE GunManagerService::getBarrelMode() const
{
    return _modeService->getMode();
}

GunManagerService::OPERATIONAL_STATUS GunManagerService::getCurrentOpStat() const
{
    return currentOpStat;
}

GunManagerService *GunManagerService::getInstance(QObject *parent,
                                                  GunCmsConfig *cmsConfig,
                                                  GunFeedbackRepository *feedbackRepo,
                                                  GunCommandRepository *cmdRepo
                                                  )
{
    if (gunManagerService == nullptr) {
        if(cmsConfig == nullptr) {
            throw ErrObjectCreation();
        }

        if(cmdRepo == nullptr) {
            throw ErrObjectCreation();
        }

        GunCommandBarrelModeService* modeService = GunCommandBarrelModeService::getInstance(
                    new HttpClientWrapper(),
                    cmsConfig,
                    cmdRepo
                    );
        GunCommandBarrelService* barrelService = GunCommandBarrelService::getInstance(
                    new HttpClientWrapper(),
                    cmsConfig,
                    cmdRepo
                    );
        // TODO : gun command status service
        gunManagerService = new GunManagerService(parent, cmsConfig, feedbackRepo, modeService, barrelService);
    }

    return gunManagerService;
}
