#include "gun_manager_service.h"
#include "src/shared/common/errors/err_object_creation.h"

GunManagerService *GunManagerService::gunManagerService = nullptr;

GunManagerService::GunManagerService(
        QObject *parent,
        GunCmsConfig *cmsConfig,
        GunFeedbackRepository *feedbackRepo,
        GunCommandBarrelModeService *modeService,
        GunCommandBarrelService *barrelService,
        GunCommandStatusService *statusService ) : QObject(parent),
    _cmsConfig(cmsConfig),
    _modeService(modeService),
    _barrelService(barrelService),
    _feedbackRepository(feedbackRepo),
    _statusService(statusService),
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
    switch (currentTechStat)
    {
    case GunManagerService::ONLINE:
    {
        bool remote = _feedbackRepository->GetStatus()->remote();
        if (remote)
        {
            switch (_modeService->getMode())
            {
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
        }
        else
        {
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
    if (currentTechStat == TECHNICAL_STATUS::OFFLINE) {
        resetOpStatus();
    }
}

void GunManagerService::setBarrelMode(GunBarrelModeEntity::MODE mode)
{
    _modeService->setModeWithConfirm(mode, true);
}

void GunManagerService::resetBarrel(bool latestConfirm)
{
    _barrelService->setBarrelWithConfirm(GunCommandBarrelRequest(0., 0.), latestConfirm);
}

void GunManagerService::setBarrel(float az, float el)
{
    _barrelService->setBarrelWithConfirm(GunCommandBarrelRequest(az, el), true);
}

void GunManagerService::resetOpStatus()
{
    if (currentOpStat != OPERATIONAL_STATUS::NOT_AVAIL) {
        currentOpStat = OPERATIONAL_STATUS::NOT_AVAIL;
        _modeService->setModeWithConfirm(GunBarrelModeEntity::NONE, false);
        resetBarrel(false);
    }
}

const GunStatusCommandEntity* GunManagerService::getCurrentStatus() const
{
    return _statusService->getCurrentStatus();
}

void GunManagerService::resetStatus()
{
    _statusService->setStatus(GunCommandStatusRequest());
}

void GunManagerService::setStatusMount(bool on)
{
    _statusService->setStatusMount(on);
}

void GunManagerService::setStatusSingleShot(bool on)
{
    _statusService->setStatusSingleShot(on);
}

bool GunManagerService::getStatusSingleShot() const
{
   return  _statusService->getCurrentStatus()->single_shot();
}

void GunManagerService::setStatusFire(bool on)
{
    _statusService->setStatusFire(on);
}

void GunManagerService::setStatusProxFuze(bool on)
{
    _statusService->setStatusProxFuze(on);
}

void GunManagerService::setStatusSiren(bool on)
{
    _statusService->setStatusSiren(on);
}

GunBarrelModeEntity::MODE GunManagerService::getBarrelMode() const
{
    return _modeService->getMode();
}

const GunBarrelEntity *GunManagerService::getCurrentBarrel() const
{
    return _feedbackRepository->GetBarrel();
}

GunManagerService::OPERATIONAL_STATUS GunManagerService::getCurrentOpStat() const
{
    return currentOpStat;
}

GunManagerService *GunManagerService::getInstance(QObject *parent,
                                                  GunCmsConfig *cmsConfig,
                                                  GunFeedbackRepository *feedbackRepo,
                                                  GunCommandRepository *cmdRepo)
{
    if (gunManagerService == nullptr)
    {
        if (cmsConfig == nullptr)
        {
            throw ErrObjectCreation();
        }

        if (cmdRepo == nullptr)
        {
            throw ErrObjectCreation();
        }

        GunCommandBarrelModeService *modeService = GunCommandBarrelModeService::getInstance(
                    new HttpClientWrapper(),
                    cmsConfig,
                    cmdRepo);
        GunCommandBarrelService *barrelService = GunCommandBarrelService::getInstance(
                    new HttpClientWrapper(),
                    cmsConfig,
                    cmdRepo);
        GunCommandStatusService *statusService = GunCommandStatusService::getInstance(
                    new HttpClientWrapper(),
                    cmsConfig,
                    cmdRepo);

        gunManagerService = new GunManagerService(parent, cmsConfig, feedbackRepo, modeService, barrelService, statusService);

        connect(modeService, &GunCommandBarrelModeService::signal_modeCheck, gunManagerService, &GunManagerService::OnBarrelModeCheck);
        connect(modeService, &GunCommandBarrelModeService::signal_setModeResponse, gunManagerService, &GunManagerService::OnBarrelModeResponse);
        connect(barrelService, &GunCommandBarrelService::signal_setBarrelResponse, gunManagerService, &GunManagerService::OnBarrelPositionResponse);
        connect(statusService, &GunCommandStatusService::signal_setStatusResponse, gunManagerService, &GunManagerService::OnStatusResponse);
    }

    return gunManagerService;
}
