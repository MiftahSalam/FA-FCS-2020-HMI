#include "osd_cms_weather_data.h"
#include "src/shared/common/errors/helper_err.h"
#include "src/shared/common/errors/err_json_parse.h"
#include "src/shared/common/errors/err_object_creation.h"
#include "src/shared/utils/utils.h"

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

    httpResponse = httpClient.put(httpReq, request.toJSON());
    connect(httpResponse, &QNetworkReply::finished, this, &OSDCMSWeatherData::onReplyFinished);
}

void OSDCMSWeatherData::onReplyFinished()
{
    QByteArray respRaw = httpResponse->readAll();

    qDebug()<<Q_FUNC_INFO<<"respRaw: "<<respRaw;
    qDebug()<<Q_FUNC_INFO<<"err: "<<httpResponse->error();

    BaseResponse<WeatherModel> resp = errorResponse(httpResponse->error());
    if(resp.getHttpCode() != 0) {
        emit signal_setWeatherResponse(resp);
        return;
    }

    resp = toResponse(respRaw);

    //TODO: update repo
    repoWeather->SetWeather(OSDWeatherEntity(
        resp.getData().getTemperature(),
        resp.getData().getHumidity(),
        resp.getData().getPressure(),
        "manual",
        "",
        OSD_MODE::MANUAL //temp
        ));

    emit signal_setWeatherResponse(resp);

}

BaseResponse<WeatherModel> OSDCMSWeatherData::toResponse(QByteArray raw)
{
    try {
        QJsonObject respObj = Utils::byteArrayToJsonObject(raw);
        int respCode = respObj["code"].toInt();
        QString respMsg = respObj["message"].toString();
        QJsonObject respData = respObj["data"].toObject();
        WeatherModel model(respData["temperature"].toDouble(),respData["humidity"].toDouble(),respData["pressure"].toDouble());
        BaseResponse<WeatherModel> resp(respCode, respMsg, model);

        qDebug()<<Q_FUNC_INFO<<"resp"<<resp.getHttpCode()<<resp.getMessage()<<resp.getData().getTemperature()<<resp.getData().getHumidity()<<resp.getData().getPressure();

        return resp;
    } catch (ErrJsonParse &e) {
        qDebug()<<Q_FUNC_INFO<<"caught error: "<<e.getMessage();
    }  catch (...) {
        qDebug()<<Q_FUNC_INFO<<"caught unkbnown error";
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
        qDebug()<<Q_FUNC_INFO<<"caught error: "<<e.getMessage();
        return BaseResponse<WeatherModel>(e.getCode(), e.getMessage(), model);
    }  catch (...) {
        qDebug()<<Q_FUNC_INFO<<"caught unkbnown error";
        ErrUnknown status;
        return BaseResponse<WeatherModel>(status.getCode(), status.getMessage(), model);
    }

    NoError status;
    return BaseResponse<WeatherModel>(status.getCode(), status.getMessage(), model);

}
