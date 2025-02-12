#include "gun_barrel_control_mode_service.h"
#include "src/shared/common/errors/err_object_creation.h"

#ifdef USE_LOG4QT
#include <log4qt/logger.h>
LOG4QT_DECLARE_STATIC_LOGGER(logger, GunBarrelControlModeService)
#else
#include <QDebug>
#endif

GunBarrelControlModeService* GunBarrelControlModeService::instance = nullptr;

GunBarrelControlModeService::GunBarrelControlModeService(
    QObject *parent, GunCommandRepository *repoGunCmd, GunCmsCommandBarrelMode *gunModeCms
    ): QObject{parent}, _repoGunCmd(repoGunCmd), _cmsGunMode(gunModeCms)
{
    synced = false;
    previousMode = repoGunCmd->GetBarrelMode()->getMode();

    timer = new QTimer(this);

    connect(timer, &QTimer::timeout, this, &GunBarrelControlModeService::onTimerTimeout);
    connect(_cmsGunMode, &GunCmsCommandBarrelMode::signal_setModeResponse, this, &GunBarrelControlModeService::onUpdateBarrelMode);

    timer->start(1000);
}

GunBarrelControlModeService *GunBarrelControlModeService::getInstance(
    QObject *parent,
    GunCommandRepository *repoGunCmd,
    GunCmsCommandBarrelMode *gunModeCms
    )
{
    if(instance == nullptr) {
        if(repoGunCmd == nullptr) {
            throw ErrObjectCreation();
        }

        if(gunModeCms == nullptr) {
            throw ErrObjectCreation();
        }

        instance = new GunBarrelControlModeService(parent, repoGunCmd, gunModeCms);
    }

    return instance;
}

void GunBarrelControlModeService::setModeWithConfirm(GunBarrelModeEntity::MODE mode, bool confirm)
{
    _repoGunCmd->SetBarrelMode(mode);
    bool modeToSend = gunBarrelModeEntityToBool(mode);
    _cmsGunMode->sendMode(confirm, modeToSend);
}

GunBarrelModeEntity::MODE GunBarrelControlModeService::getMode() const
{
    return _repoGunCmd->GetBarrelMode()->getMode();
}

void GunBarrelControlModeService::onTimerTimeout()
{
    sync();
    emit signal_modeCheck();
}

void GunBarrelControlModeService::onUpdateBarrelMode(BaseResponse<GunModeBarrelResponse> resp, bool needConfirm)
{
    if(resp.getHttpCode() == ERROR_CODE_HTTP_RESPONSE_CONNECTION_REFUSED.first) {
        _repoGunCmd->SetBarrelMode(GunBarrelModeEntity::NONE);
        previousMode = _repoGunCmd->GetBarrelMode()->getMode();
        synced = false;

        emit signal_processedResponse(resp.getData(), needConfirm);

        return;
    }

    synced = true;

    bool modeResp = resp.getData().getManualMode();
    if (!modeResp) {
        _repoGunCmd->SetBarrelMode(GunBarrelModeEntity::AUTO);
    }
    previousMode = _repoGunCmd->GetBarrelMode()->getMode();

    emit signal_processedResponse(resp.getData(), needConfirm);
}

void GunBarrelControlModeService::sync()
{
    if(!synced) {
#ifdef USE_LOG4QT
        logger()->trace()<<Q_FUNC_INFO<<" -> syncing";
#else
        qDebug()<<Q_FUNC_INFO<<"syncing";
#endif

        bool modeToSend = gunBarrelModeEntityToBool(_repoGunCmd->GetBarrelMode()->getMode());
        _cmsGunMode->sendMode(false, modeToSend);
    }
}

bool GunBarrelControlModeService::gunBarrelModeEntityToBool(GunBarrelModeEntity::MODE mode)
{
    return mode != GunBarrelModeEntity::AUTO;
}

GunBarrelModeEntity::MODE GunBarrelControlModeService::gunResponseModeToBarrelMode(bool mode)
{
    return mode ? GunBarrelModeEntity::MANUAL : GunBarrelModeEntity::AUTO;
}
