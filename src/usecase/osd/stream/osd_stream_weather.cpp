#include "osd_stream_weather.h"
#include "src/shared/common/errors/err_json_parse.h"
#include "src/shared/common/errors/err_object_creation.h"
#include "src/shared/utils/utils.h"

OSDStreamWeather* OSDStreamWeather::weatherStream = nullptr;

OSDStreamWeather::OSDStreamWeather(
    TcpMessagingOpts *config,
    OSDWeatherRepository *repoWeather,
    OSDCMSInputMode *modeService
    )
    :cfg(config), _repoWeather(repoWeather), serviceMode(modeService)
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
    return consumer->checkConnection();
}

void OSDStreamWeather::onDataReceived(QByteArray data)
{
    try {
        QJsonObject respObj = Utils::byteArrayToJsonObject(data);
        WeatherModel model(respObj["temperature"].toDouble(),respObj["humidity"].toDouble(),respObj["pressure"].toDouble());

        qDebug()<<Q_FUNC_INFO<<"data weather: temperature ->"<<model.getTemp()<<"humidity ->"<<model.getHum()<<"pressure ->"<<model.getPress();

        //check source mode manual
        if (respObj.contains("source")) {
            if (respObj["source"].toString().contains("manual")) {
                return;
            }
        }

        auto weatherMode = serviceMode->getDataMode().getWeather();
        if (!weatherMode) {
            //TODO: update repo
            _repoWeather->SetWeather(OSDWeatherEntity(
                model.getTemp(),
                model.getHum(),
                model.getPress(),
                respObj["source"].toString().toStdString(),
                respObj["status"].toString().toStdString(),
                OSD_MODE::AUTO
                ));
        }

        emit signalDataProcessed(model);
    } catch (ErrJsonParse &e) {
        qDebug()<<Q_FUNC_INFO<<"caught error: "<<e.getMessage();
    }  catch (...) {
        qDebug()<<Q_FUNC_INFO<<"caught unkbnown error";
    }
}
