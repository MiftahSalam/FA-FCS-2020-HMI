#include "osd_stream_weather.h"
#include "src/shared/common/errors/err_json_parse.h"
#include "src/shared/common/errors/err_object_creation.h"
#include "src/shared/common/errors/err_osd_data.h"
#include "src/shared/utils/utils.h"

OSDStreamWeather* OSDStreamWeather::weatherStream = nullptr;

OSDStreamWeather::OSDStreamWeather(
    TcpMessagingOpts *config,
    OSDWeatherRepository *repoWeather,
    OSDCMSInputMode *modeService
    )
    :cfg(config), _repoWeather(repoWeather), serviceMode(modeService), currentErr(NoError())
{
    consumer = new TcpMessagingWrapper(this, config);
    connect(consumer, &TcpMessagingWrapper::signalForwardMessage, this, &OSDStreamWeather::onDataReceived);
}

OSDStreamWeather *OSDStreamWeather::getInstance(
    TcpMessagingOpts *config,
    OSDWeatherRepository *repoWeather,
    OSDCMSInputMode *modeService
    )
{
    if (weatherStream == nullptr) {
        if(config == nullptr) {
            throw ErrObjectCreation();
        }

        if(repoWeather == nullptr) {
            throw ErrObjectCreation();
        }

        if(modeService == nullptr) {
            throw ErrObjectCreation();
        }
        weatherStream = new OSDStreamWeather(config, repoWeather, modeService);
    }
    return weatherStream;
}

BaseError OSDStreamWeather::check()
{
    //TODO: check no data error, invalid data error, etc
    // return consumer->checkConnection();
    auto connError = consumer->checkConnection();
    if (connError.getCode() != 0){
        currentErr = static_cast<BaseError>(connError);
        return currentErr;
    }
    return currentErr;
}

void OSDStreamWeather::onDataReceived(QByteArray data)
{
    try {
        QJsonObject respObj = Utils::byteArrayToJsonObject(data);
        WeatherModel model(respObj["temperature"].toDouble(),respObj["pressure"].toDouble(),respObj["humidity"].toDouble());

        qDebug()<<Q_FUNC_INFO<<"data weather: temperature ->"<<model.getTemperature()<<"humidity ->"<<model.getHumidity()<<"pressure ->"<<model.getPressure();

        //check source mode manual
        if (respObj.contains("source")) {
            if (respObj["source"].toString().contains("manual")) {
                return;
            }
        }

        auto weatherMode = serviceMode->getDataMode().getWeather();
        if (!weatherMode && check().getCode() == ERROR_NO.first) {
            _repoWeather->SetWeather(OSDWeatherEntity(
                model.getTemperature(),
                model.getPressure(),
                model.getHumidity(),
                respObj["source"].toString().toStdString(),
                respObj["status"].toString().toStdString(),
                OSD_MODE::AUTO
                ));
        }
        handleError(respObj["status"].toString());

        emit signalDataProcessed(model);
    } catch (ErrJsonParse &e) {
        qDebug()<<Q_FUNC_INFO<<"caught error: "<<e.getMessage();
    }  catch (...) {
        qDebug()<<Q_FUNC_INFO<<"caught unkbnown error";
    }
}

void OSDStreamWeather::handleError(const QString &err)
{
    if (err.toStdString().empty()) {
        currentErr = NoError();
    } else if (err.contains("Partially")) {
        currentErr = ErrOSDDataPartiallyInvalid();
    } else if (err.contains("Range")) {
        currentErr = ErrOSDDataOutOfRange();
    } else {
        currentErr = ErrOSDDataInvalid();
    }
}
