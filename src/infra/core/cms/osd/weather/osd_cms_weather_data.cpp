#include "osd_cms_weather_data.h"
#include "src/shared/common/errors/helper_err.h"
#include "src/shared/common/errors/err_json_parse.h"
#include "src/shared/common/errors/err_object_creation.h"
#include "src/shared/utils/utils.h"

#ifdef USE_LOG4QT
#include <log4qt/logger.h>
LOG4QT_DECLARE_STATIC_LOGGER(logger, OSDCMSWeatherData)
#else
#include <QDebug>
#endif

OSDCMSWeatherData* OSDCMSWeatherData::weatherData = nullptr;

OSDCMSWeatherData::OSDCMSWeatherData(
        HttpClientWrapper *parent,
        OSDCmsConfig *cmsConfig,
        OSDWeatherRepository *repoWeather
        ): HttpClientWrapper(parent), cfgCms(cmsConfig), repoWeather(repoWeather)
{
    if(parent == nullptr) {
        throw ErrObjectCreation();
    }
}

OSDCMSWeatherData *OSDCMSWeatherData::getInstance(
        HttpClientWrapper *httpClient = nullptr,
        OSDCmsConfig *cmsConfig = nullptr,
        OSDWeatherRepository *repoWeather
        )
{
    if (weatherData == nullptr) {
        if(cmsConfig == nullptr) {
            throw ErrObjectCreation();
        }

        if(httpClient == nullptr) {
            throw ErrObjectCreation();
        }

        if(repoWeather == nullptr) {
            throw ErrObjectCreation();
        }

        weatherData = new OSDCMSWeatherData(httpClient, cmsConfig, repoWeather);
    }
    return weatherData;

}

void OSDCMSWeatherData::set(OSDSetWeatherRequest request)
{
    QNetworkRequest httpReq = QNetworkRequest(cfgCms->getInstance("")->getManualDataUrl()+"/weather");
    httpReq.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    auto httpResponse = httpClient.put(httpReq, request.toJSON());
    connect(httpResponse, &QNetworkReply::finished, this, &OSDCMSWeatherData::onReplyFinished);
}

void OSDCMSWeatherData::onReplyFinished()
{
    QNetworkReply *httpResponse = dynamic_cast<QNetworkReply *>(sender());
    QByteArray respRaw = httpResponse->readAll();

#ifdef USE_LOG4QT
    logger()->debug()<<Q_FUNC_INFO<<" -> respRaw: "<<respRaw;
    logger()->debug()<<Q_FUNC_INFO<<" -> err: "<<httpResponse->error();
#else
    qDebug()<<Q_FUNC_INFO<<"respRaw: "<<respRaw;
    qDebug()<<Q_FUNC_INFO<<"err: "<<httpResponse->error();
#endif

    BaseResponse<WeatherModel> resp = errorResponse(httpResponse->error());
    if(resp.getHttpCode() != 0) {
        emit signal_setWeatherResponse(resp);
        return;
    }

    resp = toResponse(respRaw);

    repoWeather->SetWeather(OSDWeatherEntity(
                                resp.getData().getTemperature(),
                                resp.getData().getPressure(),
                                resp.getData().getHumidity(),
                                "manual",
                                "",
                                OSD_MODE::MANUAL
                                ));

    emit signal_setWeatherResponse(resp);

    httpResponse->deleteLater();
}

BaseResponse<WeatherModel> OSDCMSWeatherData::toResponse(QByteArray raw)
{
    try {
        QJsonObject respObj = Utils::byteArrayToJsonObject(raw);
        int respCode = respObj["code"].toInt();
        QString respMsg = respObj["message"].toString();
        QJsonObject respData = respObj["data"].toObject();
        WeatherModel model(respData["temperature"].toDouble(),respData["pressure"].toDouble(),respData["humidity"].toDouble());
        BaseResponse<WeatherModel> resp(respCode, respMsg, model);

#ifdef USE_LOG4QT
        logger()->debug()<<Q_FUNC_INFO<<" -> resp. http code: "<<resp.getHttpCode()
                        <<", message: "<<resp.getMessage()
                       <<", temp: "<<resp.getData().getTemperature()
                      <<", hum: "<<resp.getData().getHumidity()
                     <<", press: "<<resp.getData().getPressure()
                       ;
#else
        qDebug()<<Q_FUNC_INFO<<"resp"<<resp.getHttpCode()<<resp.getMessage()<<resp.getData().getTemperature()<<resp.getData().getHumidity()<<resp.getData().getPressure();
#endif

        return resp;
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

    ErrUnknown status;
    WeatherModel model(0, 0, 0);
    return BaseResponse<WeatherModel>(status.getCode(), status.getMessage(), model);
}

BaseResponse<WeatherModel> OSDCMSWeatherData::errorResponse(QNetworkReply::NetworkError err)
{
    WeatherModel model(0, 0, 0);
    try {
        ErrHelper::throwHttpError(err);
    } catch (BaseError &e) {
#ifdef USE_LOG4QT
        logger()->error()<<Q_FUNC_INFO<<" -> caught error: "<<e.getMessage();
#else
        qWarning()<<Q_FUNC_INFO<<"caught error: "<<e.getMessage();
#endif
        return BaseResponse<WeatherModel>(e.getCode(), e.getMessage(), model);
    }  catch (...) {
#ifdef USE_LOG4QT
        logger()->error()<<Q_FUNC_INFO<<" -> caught unkbnown error";
#else
        qWarning()<<Q_FUNC_INFO<<"caught unkbnown error";
#endif
        ErrUnknown status;
        return BaseResponse<WeatherModel>(status.getCode(), status.getMessage(), model);
    }

    NoError status;
    return BaseResponse<WeatherModel>(status.getCode(), status.getMessage(), model);

}
