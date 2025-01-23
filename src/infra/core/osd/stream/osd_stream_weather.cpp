#include "osd_stream_weather.h"
#include "src/shared/common/errors/err_json_parse.h"
#include "src/shared/common/errors/err_object_creation.h"
#include "src/shared/common/errors/err_osd_data.h"
#include "src/shared/utils/utils.h"

#ifdef USE_LOG4QT
#include <log4qt/logger.h>
LOG4QT_DECLARE_STATIC_LOGGER(logger, OSDStreamWeather)
#else
#include <QDebug>
#endif

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

#ifdef USE_LOG4QT
        logger()->trace()<<Q_FUNC_INFO<<" -> temp: "<<model.getTemperature()
                        <<", hum: "<<model.getHumidity()
                       <<", press: "<<model.getPressure()
                         ;
#else
        qDebug()<<Q_FUNC_INFO<<"data weather: temperature ->"<<model.getTemperature()<<"humidity ->"<<model.getHumidity()<<"pressure ->"<<model.getPressure();
#endif

        //check source mode manual
        if (respObj.contains("source")) {
            if (respObj["source"].toString().contains("manual")) {
                return;
            }
        }

        auto weatherMode = serviceMode->getDataMode()->weather();
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
#ifdef USE_LOG4QT
        logger()->error()<<Q_FUNC_INFO<<" -> caught error: "<<e.getMessage();
#else
        qWarning()<<Q_FUNC_INFO<<"caught error: "<<e.getMessage();
#endif
    }  catch (...) {
#ifdef USE_LOG4QT
        logger()->error()<<Q_FUNC_INFO<<" -> caught unkbnown error";
#else
        qWarning()<<Q_FUNC_INFO<<"caught unkbnown error";
#endif
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
