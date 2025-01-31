#include "osd_service.h"
#include "src/shared/common/errors/err_object_creation.h"

#ifdef USE_LOG4QT
#include <log4qt/logger.h>
LOG4QT_DECLARE_STATIC_LOGGER(logger, OSDService)
#else
#include <QDebug>
#endif

OSDService* OSDService::instance = nullptr;

OSDService::OSDService(QObject *parent, OSDRepository *osdRepo, OSDCMS* osdCms, OSDStream* osdStream)
    : QObject{parent}, repoOsd(osdRepo), cmsOsd(osdCms), streamOsd(osdStream)
{
    repoMode = repoOsd->getRepoOSDMode();

    previousMode = new OSDInputModeEntity(
        repoMode->GetMode()->inersia(),
        repoMode->GetMode()->position(),
        repoMode->GetMode()->speed(),
        repoMode->GetMode()->waterSpeed(),
        repoMode->GetMode()->weather(),
        repoMode->GetMode()->wind()
        );

    synced = false;
    timer = new QTimer(this);

    connect(cmsOsd->getServiceOSDCMSMode(), &OSDCMSInputMode::signal_setModeResponse, this, &OSDService::onUpdateInputMode);
    connect(cmsOsd->getServiceOSDCMSGyro(), &OSDCMSGyroData::signal_setGyroResponse, this, &OSDService::onUpdateManualDataGyro);
    connect(cmsOsd->getServiceOSDCMSPosition(), &OSDCMSPositionData::signal_setPositionResponse, this, &OSDService::onUpdateManualDataPosition);
    connect(cmsOsd->getServiceOSDCMSSpeed(), &OSDCMSSpeedData::signal_setSpeedResponse, this, &OSDService::onUpdateManualDataSpeed);
    connect(cmsOsd->getServiceOSDCMSWaterSpeed(), &OSDCMSWaterSpeedData::signal_setWaterSpeedResponse, this, &OSDService::onUpdateManualDataWaterSpeed);
    connect(cmsOsd->getServiceOSDCMSWeather(), &OSDCMSWeatherData::signal_setWeatherResponse, this, &OSDService::onUpdateManualDataWeather);
    connect(cmsOsd->getServiceOSDCMSWind(), &OSDCMSWindData::signal_setWindResponse, this, &OSDService::onUpdateManualDataWind);

    connect(timer, &QTimer::timeout, this, &OSDService::onTimerTimeout);

    timer->start(1000);
}

void OSDService::onTimerTimeout()
{
    sync();
}

OSDService* OSDService::getInstance(
    QObject* parent,
    OSDRepository *osdRepo,
    OSDCMS* osdCms,
    OSDStream* osdStream
    ) {
    if(instance == nullptr) {
        if(osdRepo == nullptr) {
            throw ErrObjectCreation();
        }

        if(osdCms == nullptr) {
            throw ErrObjectCreation();
        }

        instance = new OSDService(parent, osdRepo, osdCms);
    }

    return instance;
}

void OSDService::onUpdateManualDataGyro(BaseResponse<GyroModel> resp)
{
    if(resp.getHttpCode() != ERROR_NO.first) {
        emit signal_processedSetResponseGyro(resp.getData());
        return;
    }

    repoOsd->getRepoOSDInertia()->SetInertia(OSDInertiaEntity(
        resp.getData().getHeading(),
        resp.getData().getPicth(),
        resp.getData().getRoll(),
        "manual",
        "",
        OSD_MODE::MANUAL
        ));

    emit signal_processedSetResponseGyro(resp.getData());
}

void OSDService::onUpdateManualDataPosition(BaseResponse<PositionModel> resp)
{
    if(resp.getHttpCode() != ERROR_NO.first) {
        emit signal_processedSetResponsePosition(resp.getData());
        return;
    }

    repoOsd->getRepoOSDPosition()->SetPosition(OSDPositionEntity(
        resp.getData().getLatitude(),
        resp.getData().getLongitude(),
        "manual",
        "",
        OSD_MODE::MANUAL
        ));

    emit signal_processedSetResponsePosition(resp.getData());
}

void OSDService::onUpdateManualDataSpeed(BaseResponse<SpeedModel> resp)
{
    if(resp.getHttpCode() != ERROR_NO.first) {
        emit signal_processedSetResponseSpeed(resp.getData());
        return;
    }

    repoOsd->getRepoOSDSpeed()->SetSpeed(OSDSpeedEntity(
        resp.getData().getSpeed(),
        resp.getData().getCourse(),
        "manual",
        "",
        OSD_MODE::MANUAL
        ));

    emit signal_processedSetResponseSpeed(resp.getData());
}

void OSDService::onUpdateManualDataWaterSpeed(BaseResponse<WaterSpeedModel> resp)
{
    if(resp.getHttpCode() != ERROR_NO.first) {
        emit signal_processedSetResponseWaterSpeed(resp.getData());
        return;
    }

    repoOsd->getRepoOSDWaterSpeed()->SetWaterSpeed(OSDWaterSpeedEntity(
        resp.getData().getSpeed(),
        resp.getData().getCourse(),
        "manual",
        "",
        OSD_MODE::MANUAL
        ));

    emit signal_processedSetResponseWaterSpeed(resp.getData());
}

void OSDService::onUpdateManualDataWeather(BaseResponse<WeatherModel> resp)
{
    if(resp.getHttpCode() != ERROR_NO.first) {
        emit signal_processedSetResponseWeather(resp.getData());
        return;
    }

    repoOsd->getRepoOSDWeather()->SetWeather(OSDWeatherEntity(
        resp.getData().getTemperature(),
        resp.getData().getPressure(),
        resp.getData().getHumidity(),
        "manual",
        "",
        OSD_MODE::MANUAL
        ));

    emit signal_processedSetResponseWeather(resp.getData());
}

void OSDService::onUpdateManualDataWind(BaseResponse<WindModel> resp)
{
    if(resp.getHttpCode() != ERROR_NO.first) {
        emit signal_processedSetResponseWind(resp.getData());
        return;
    }

    repoOsd->getRepoOSDWind()->SetWind(OSDWindEntity(
        resp.getData().getSpeed(),
        resp.getData().getDirection(),
        "manual",
        "",
        OSD_MODE::MANUAL
        ));

    emit signal_processedSetResponseWind(resp.getData());
}

void OSDService::onUpdateInputMode(const QString datafisis, BaseResponse<InputModeModel> resp, bool needConfirm)
{
    if(resp.getHttpCode() == ERROR_CODE_HTTP_RESPONSE_CONNECTION_REFUSED.first) {
        resetToPrevMode();
        synced = false;

        emit signal_processedSetModeResponse(datafisis, resp, needConfirm);

        return;
    }

    synced = true;

    previousMode->setInersiaMode(repoMode->GetMode()->inersia());
    previousMode->setPositionMode(repoMode->GetMode()->position());
    previousMode->setSpeedMode(repoMode->GetMode()->speed());
    previousMode->setWaterSpeedMode(repoMode->GetMode()->waterSpeed());
    previousMode->setWeatherMode(repoMode->GetMode()->weather());
    previousMode->setWindMode(repoMode->GetMode()->wind());

    emit signal_processedSetModeResponse(datafisis, resp, needConfirm);
}

void OSDService::onUpdateAutoDataPosition(PositionModel data)
{
    auto positionMode = getDataMode()->position();
    auto streamPos = streamOsd->getServiceOSDStreamPosition();
    auto _repoPos = repoOsd->getRepoOSDPosition();
    if (!positionMode && streamPos->check().getCode() == ERROR_NO.first) {
        _repoPos->SetPosition(OSDPositionEntity(
            data.getLatitude(),
            data.getLongitude(),
            data.source(),
            data.status(),
            data.mode()
            ));
    }
}

void OSDService::setDataMode(const QString &dataFisis, const OSD_MODE mode)
{
    OSDInputModeEntity curMode(
        repoMode->GetMode()->position(),
        repoMode->GetMode()->speed(),
        repoMode->GetMode()->inersia(),
        repoMode->GetMode()->waterSpeed(),
        repoMode->GetMode()->wind(),
        repoMode->GetMode()->weather()
        );

    if (dataFisis == "position") {
        curMode.setPositionMode(mode);
    } else if (dataFisis == "inertia") {
        curMode.setInersiaMode(mode);
    } else if (dataFisis == "water_speed") {
        curMode.setWaterSpeedMode(mode);
    } else if (dataFisis == "speed"){
        curMode.setSpeedMode(mode);
    } else if (dataFisis == "wind"){
        curMode.setWindMode(mode);
    }else if (dataFisis == "weather"){
        curMode.setWeatherMode(mode);
    }else{
        return;
        // TODO: handle invalid datafisis
    }

    repoMode->SetMode(curMode);

    cmsOsd->getServiceOSDCMSMode()->setDataMode(true, dataFisis, OSDInputModeRequest(
                                                                     repoMode->GetMode()->position(),
                                                                     repoMode->GetMode()->speed(),
                                                                     repoMode->GetMode()->inersia(),
                                                                     repoMode->GetMode()->waterSpeed(),
                                                                     repoMode->GetMode()->wind(),
                                                                     repoMode->GetMode()->weather()
                                                                     ));
}

const OSDInputModeEntity *OSDService::getDataMode() const
{
    return repoMode->GetMode();
}

void OSDService::setManualDataInertia(OSDSetGyroRequest req)
{
    cmsOsd->getServiceOSDCMSGyro()->set(req);
}

void OSDService::setManualDataSpeed(OSDSetSpeedRequest req)
{
    cmsOsd->getServiceOSDCMSSpeed()->set(req);
}

void OSDService::setManualDataPosition(OSDSetPositionRequest req)
{
    cmsOsd->getServiceOSDCMSPosition()->set(req);
}

void OSDService::setManualDataWaterSpeed(OSDSetWaterSpeedRequest req)
{
    cmsOsd->getServiceOSDCMSWaterSpeed()->set(req);
}

void OSDService::setManualDataWeather(OSDSetWeatherRequest req)
{
    cmsOsd->getServiceOSDCMSWeather()->set(req);
}

void OSDService::setManualDataWind(OSDSetWindRequest req)
{
    cmsOsd->getServiceOSDCMSWind()->set(req);
}

float OSDService::getOSDHeading() const
{
    return repoOsd->getRepoOSDInertia()->GetInertia()->heading();
}

BaseError OSDService::getOSDAutoStatusInertia() const
{
    return streamOsd->getServiceOSDStreamGyro()->check();
}

BaseError OSDService::getOSDAutoStatusSpeed() const
{
    return streamOsd->getServiceOSDStreamSpeed()->check();
}

BaseError OSDService::getOSDAutoStatusWind() const
{
    return streamOsd->getServiceOSDStreamWind()->check();
}

BaseError OSDService::getOSDAutoStatusPosition() const
{
    return streamOsd->getServiceOSDStreamPosition()->check();
}

BaseError OSDService::getOSDAutoStatusWaterSpeed() const
{
    return streamOsd->getServiceOSDStreamWaterSpeed()->check();
}

BaseError OSDService::getOSDAutoStatusWeather() const
{
    return streamOsd->getServiceOSDStreamWeather()->check();
}

void OSDService::sync()
{
    if(!synced) {
#ifdef USE_LOG4QT
        logger()->trace()<<Q_FUNC_INFO<<" -> syncing";
#else
        qDebug()<<Q_FUNC_INFO<<"syncing";
#endif
        cmsOsd->getServiceOSDCMSMode()->setDataMode(false, "", OSDInputModeRequest(
                                                                   repoMode->GetMode()->position(),
                                                                   repoMode->GetMode()->speed(),
                                                                   repoMode->GetMode()->inersia(),
                                                                   repoMode->GetMode()->waterSpeed(),
                                                                   repoMode->GetMode()->wind(),
                                                                   repoMode->GetMode()->weather()
                                                                   ));
    }
}

void OSDService::resetToPrevMode()
{
    repoMode->SetMode(*previousMode);
}

OSDService::~OSDService() {

}
