#include "gun_manager_service.h"
#include "src/shared/common/errors/err_object_creation.h"

GunManagerService *GunManagerService::gunManagerService = nullptr;

GunManagerService::GunManagerService(
    QObject *parent,
    GunCmsConfig *cmsConfig,
    GunFeedbackRepository *feedbackRepo,
    GunCommandRepository *cmdRepo,
    GunCoverageRepository *coverageRepo,
    GunBarrelControlModeService *modeService,
    GunCmsCommandBarrel *barrelCms,
    GunCmsCommandStatus *statusCms ,
    GunStream *gunStream) :
    QObject(parent),
    _cmsConfig(cmsConfig),
    _modeService(modeService),
    _barrelService(barrelCms),
    _repoFeedback(feedbackRepo),
    _repoGunCmd(cmdRepo),
    _repoGunCoverage(coverageRepo),
    _statusCms(statusCms),
    _streamGun(gunStream),
    currentOpStat(GunManagerService::NOT_AVAIL),
    currentTechStat(GunManagerService::OFFLINE)
{
    timer = new QTimer(this);

    connect(_streamGun->getStreamGunStatus(), &GunFeedbackStatusStream::signalDataProcessed,
            this, &GunManagerService::updateGunFeedbackStatus);
    connect(_streamGun->getStreamGunBarrel(), &GunFeedbackBarrelStream::signalDataProcessed,
            this, &GunManagerService::updateGunFeedbackBarrel);
    connect(_streamGun->getStreamGunCoverage(), &GunCoverageStream::signalDataProcessed,
            this, &GunManagerService::updateGunFeedbackCoverage);

    connect(timer, &QTimer::timeout, this, &GunManagerService::onTimerTImeout);

    timer->start(1000);
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
        bool remote = _repoFeedback->GetStatus()->remote();
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
    return _repoGunCmd->GetStatus();
}

void GunManagerService::resetStatus()
{
    _repoFeedback->SetStatus(GunStatusFeedbackEntity());
}

void GunManagerService::setStatusMount(bool on)
{
    auto curStatus = _repoGunCmd->GetStatus();
    _statusCms->setStatus(GunCommandStatusRequest(
        on,
        curStatus->single_shot(),
        curStatus->fire(),
        curStatus->proximity(),
        curStatus->siren()
        ));
}

void GunManagerService::setStatusSingleShot(bool on)
{
    auto curStatus = _repoGunCmd->GetStatus();
    _statusCms->setStatus(GunCommandStatusRequest(
        curStatus->mount(),
        on,
        curStatus->fire(),
        curStatus->proximity(),
        curStatus->siren()
        ));
}

bool GunManagerService::getStatusSingleShot() const
{
    return  _repoGunCmd->GetStatus()->single_shot();
}

void GunManagerService::setStatusFire(bool on)
{
    auto curStatus = _repoGunCmd->GetStatus();
    _statusCms->setStatus(GunCommandStatusRequest(
        curStatus->mount(),
        curStatus->single_shot(),
        on,
        curStatus->proximity(),
        curStatus->siren()
        ));
}

void GunManagerService::setStatusProxFuze(bool on)
{
    auto curStatus = _repoGunCmd->GetStatus();
    _statusCms->setStatus(GunCommandStatusRequest(
        curStatus->mount(),
        curStatus->single_shot(),
        curStatus->fire(),
        on,
        curStatus->siren()
        ));
}

void GunManagerService::setStatusSiren(bool on)
{
    auto curStatus = _repoGunCmd->GetStatus();
    _statusCms->setStatus(GunCommandStatusRequest(
        curStatus->mount(),
        curStatus->single_shot(),
        curStatus->fire(),
        curStatus->proximity(),
        on
        ));
}

void GunManagerService::updateGunCommandStatus(BaseResponse<GunCommandStatusResponse> resp, bool needConfirm)
{
    if(resp.getHttpCode() == ERROR_NO.first) {
        _repoGunCmd->SetStatus(GunStatusCommandEntity(
            resp.getData().getMount(),
            resp.getData().getSingleShot(),
            resp.getData().getFireOrder(),
            resp.getData().getProxFuze(),
            resp.getData().getSiren()
            ));
    }

    emit OnStatusResponse(resp.getData(), needConfirm);
}

void GunManagerService::updateGunCommandBarrel(BaseResponse<GunCommandBarrelResponse> resp, bool needConfirm)
{
    if(resp.getHttpCode() == ERROR_NO.first) {
        _repoGunCmd->SetBarrel(resp.getData().getAzimuth(), resp.getData().getElevation());
    }

    emit OnBarrelPositionResponse(resp.getData(), needConfirm);
}

void GunManagerService::updateGunFeedbackStatus(GunFeedbackStatusModel data)
{
    auto stream = _streamGun->getStreamGunStatus();
    if (stream->check().getCode() == ERROR_NO.first) {
        _repoFeedback->SetStatus(GunStatusFeedbackEntity(
            data.getOpMode(),
            data.getRemote(),
            data.getMount(),
            data.getBarrelTemperature(),
            data.getGunReadyToStart(),
            data.getGunReadyToFire(),
            data.getFireMode(),
            data.getBlindArc(),
            data.getMissAlignment(),
            data.getMagazine()
            ));
    }

    emit signal_processedGunStatus(data);
}

void GunManagerService::updateGunFeedbackBarrel(GunFeedbackBarrelModel data)
{
    auto stream = _streamGun->getStreamGunBarrel();
    if (stream->check().getCode() == ERROR_NO.first) {
        _repoFeedback->SetBarrel(
            data.getAzimuth(),
            data.getElevation()
            );
    }

    emit signal_processedGunBarrel(data);
}

void GunManagerService::updateGunFeedbackCoverage(GunCoverageModel data)
{
    auto stream = _streamGun->getStreamGunCoverage();
    if (stream->check().getCode() == ERROR_NO.first) {
        _repoGunCoverage->SetGunCoverage(GunCoverageEntity(
            data.getMaxRange(),
            data.getBlindArc(),
            data.getOrientation()
            ));
    }

    emit signal_processedGunCoverage(data);
}

void GunManagerService::onTimerTImeout()
{
    _streamGun->getStreamGunCoverage()->check();
}

GunBarrelModeEntity::MODE GunManagerService::getBarrelMode() const
{
    return _modeService->getMode();
}

const GunBarrelEntity *GunManagerService::getCurrentBarrel() const
{
    return _repoFeedback->GetBarrel();
}

GunManagerService::OPERATIONAL_STATUS GunManagerService::getCurrentOpStat() const
{
    return currentOpStat;
}

GunManagerService *GunManagerService::getInstance(QObject *parent,
                                                  GunCmsConfig *cmsConfig,
                                                  GunFeedbackRepository *feedbackRepo,
                                                  GunCommandRepository *cmdRepo,
                                                  GunCoverageRepository *coverageRepo,
                                                  GunBarrelControlModeService *modeService,
                                                  GunStream *gunStream
                                                  )
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

        if (modeService == nullptr)
        {
            throw ErrObjectCreation();
        }

        if (gunStream == nullptr)
        {
            throw ErrObjectCreation();
        }

        if (coverageRepo == nullptr)
        {
            throw ErrObjectCreation();
        }

        GunCmsCommandBarrel *cmsBarrel = GunCmsCommandBarrel::getInstance(
            new HttpClientWrapper(),
            cmsConfig
            );
        GunCmsCommandStatus *cmsStatus = GunCmsCommandStatus::getInstance(
            new HttpClientWrapper(),
            cmsConfig
            );

        gunManagerService = new GunManagerService(parent, cmsConfig, feedbackRepo, cmdRepo, coverageRepo, modeService, cmsBarrel, cmsStatus, gunStream);

        connect(modeService, &GunBarrelControlModeService::signal_modeCheck, gunManagerService, &GunManagerService::OnBarrelModeCheck);
        connect(modeService, &GunBarrelControlModeService::signal_processedResponse, gunManagerService, &GunManagerService::OnBarrelModeResponse);
        connect(cmsBarrel, &GunCmsCommandBarrel::signal_setBarrelResponse, gunManagerService, &GunManagerService::updateGunCommandBarrel);
        connect(cmsStatus, &GunCmsCommandStatus::signal_setStatusResponse, gunManagerService, &GunManagerService::updateGunCommandStatus);
    }

    return gunManagerService;
}

BaseError GunManagerService::getGunBarrelError() const
{
    return _streamGun->getStreamGunBarrel()->check();
}

BaseError GunManagerService::getGunStatusError() const
{
    return _streamGun->getStreamGunStatus()->check();
}

const GunCoverageEntity *GunManagerService::getCoverage() const
{
    return _repoGunCoverage->GetGunCoverage();
}
