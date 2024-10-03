#include "osd_cms_input_mode.h"
#include "src/shared/common/errors/helper_err.h"
#include "src/shared/common/errors/err_json_parse.h"
#include "src/shared/common/errors/err_object_creation.h"
#include "src/shared/utils/utils.h"

#ifdef USE_LOG4QT
#include <log4qt/logger.h>
LOG4QT_DECLARE_STATIC_LOGGER(logger, OSDCMSInputMode)
#else
#include <QDebug>
#endif

OSDCMSInputMode* OSDCMSInputMode::inputMode = nullptr;

OSDCMSInputMode::OSDCMSInputMode(
        HttpClientWrapper *parent,
        OSDCmsConfig *cmsConfig
        ):
    HttpClientWrapper(parent),
    currentMode(OSDInputModeRequest(false, false, false, false, false, false)),
    previousMode(OSDInputModeRequest(false, false, false, false, false, false)),
    cfgCms(cmsConfig)
{
    synced = false;

    if(parent == nullptr) {
        throw ErrObjectCreation();
    }

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &OSDCMSInputMode::onTimerTimeout);

    timer->start(1000);
}

OSDCMSInputMode *OSDCMSInputMode::getInstance(
        HttpClientWrapper *httpClient = nullptr,
        OSDCmsConfig *cmsConfig = nullptr,
        OSDBaseRepository *repoPos =nullptr
        )
{
    if (inputMode == nullptr) {
        if(cmsConfig == nullptr) {
            throw ErrObjectCreation();
        }

        if(httpClient == nullptr) {
            throw ErrObjectCreation();
        }

        if(repoPos == nullptr) {
            throw ErrObjectCreation();
        }

        inputMode = new OSDCMSInputMode(httpClient, cmsConfig);
    }

    return inputMode;
}

void OSDCMSInputMode::set(OSDInputModeRequest request)
{
    QNetworkRequest httpReq = QNetworkRequest(cfgCms->getInstance("")->getModeUrl());
    httpReq.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    httpResponse = httpClient.put(httpReq, request.toJSON());
    connect(httpResponse, &QNetworkReply::finished, this, &OSDCMSInputMode::onReplyFinished);
}

void OSDCMSInputMode::setDataMode(const QString &dataFisis, const bool manualMode)
{
    if (dataFisis == "position") {
        currentMode.setPosition(manualMode);
    } else if (dataFisis == "inertia") {
        currentMode.setInersia(manualMode);
    } else if (dataFisis == "water_speed") {
        currentMode.setWaterSpeed(manualMode);
    } else if (dataFisis == "speed"){
        currentMode.setSpeed(manualMode);
    } else if (dataFisis == "wind"){
        currentMode.setWind(manualMode);
    }else if (dataFisis == "weather"){
        currentMode.setWeather(manualMode);
    }else{
        return;
        // TODO: handle invalid datafisis
    }

    requestSync = false;
    lastUpdateMode = dataFisis;

    set(currentMode);
}

const OSDInputModeRequest OSDCMSInputMode::getDataMode() const
{
    return currentMode;
}

void OSDCMSInputMode::sync()
{
    if(!synced) {
#ifdef USE_LOG4QT
        logger()->trace()<<Q_FUNC_INFO<<" -> syncing";
#else
        qDebug()<<Q_FUNC_INFO<<"syncing";
#endif

        lastUpdateMode = "";
        requestSync = true;
        set(currentMode);
    }
}

void OSDCMSInputMode::onReplyFinished()
{
    QByteArray respRaw = httpResponse->readAll();
    auto respErr = httpResponse->error();

#ifdef USE_LOG4QT
    logger()->debug()<<Q_FUNC_INFO<<" -> respRaw: "<<respRaw;
    logger()->debug()<<Q_FUNC_INFO<<" -> err: "<<(int)respErr;
#else
    qDebug()<<Q_FUNC_INFO<<"respRaw: "<<respRaw;
    qDebug()<<Q_FUNC_INFO<<"err: "<<httpResponse->error();
#endif

    BaseResponse<InputModeModel> resp = errorResponse(httpResponse->error());
    if(resp.getHttpCode() != 0) {
        resetToPrevMode();
        synced = false;

        emit signal_setModeResponse(lastUpdateMode, resp, !requestSync);

        return;
    }

    synced = true;
    resp = toResponse(respRaw);

    previousMode = currentMode;
    //TODO: update repo
    //    repoInputMode->SetEntity(); //temp

    emit signal_setModeResponse(lastUpdateMode, resp, !requestSync);
}

void OSDCMSInputMode::onTimerTimeout()
{
    sync();
}

BaseResponse<InputModeModel> OSDCMSInputMode::toResponse(QByteArray raw)
{
    try {
        QJsonObject respObj = Utils::byteArrayToJsonObject(raw);
        int respCode = respObj["code"].toInt();
        QString respMsg = respObj["message"].toString();
        QJsonObject respData = respObj["data"].toObject();
        InputModeModel model(
            respData["position"].toBool(),
            respData["speed"].toBool(),
            respData["inertia"].toBool(),
            respData["water_speed"].toBool(),
            respData["wind"].toBool(),
            respData["weather"].toBool()
                );
        BaseResponse<InputModeModel> resp(respCode, respMsg, model);

#ifdef USE_LOG4QT
        logger()->debug()<<Q_FUNC_INFO<<" -> resp. http code: "<<resp.getHttpCode()
                        <<", message: "<<resp.getMessage()
                       <<", position: "<<resp.getData().getPosition()
                       <<", speed: "<<resp.getData().getSpeed()
                       <<", inertia: "<<resp.getData().getInersia()
                      <<", water_speed: "<<resp.getData().getWaterSpeed()
                     <<", wind: "<<resp.getData().getWind()
                    <<", weather: "<<resp.getData().getWeather()
                         ;
#else
        qDebug()<<Q_FUNC_INFO<<"resp"<<resp.getHttpCode()<<resp.getMessage()<<resp.getData().getHeading()<<resp.getData().getPicth()<<resp.getData().getRoll();
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
    InputModeModel model(false, false, false, false, false, false);
    return BaseResponse<InputModeModel>(status.getCode(), status.getMessage(), model);
}

BaseResponse<InputModeModel> OSDCMSInputMode::errorResponse(QNetworkReply::NetworkError err)
{
    InputModeModel model(false, false, false, false, false, false);
    try {
        ErrHelper::throwHttpError(err);
    } catch (BaseError &e) {
#ifdef USE_LOG4QT
        logger()->error()<<Q_FUNC_INFO<<" -> caught error: "<<e.getMessage();
#else
        qWarning()<<Q_FUNC_INFO<<"caught error: "<<e.getMessage();
#endif
        return BaseResponse<InputModeModel>(e.getCode(), e.getMessage(), model);
    }  catch (...) {
#ifdef USE_LOG4QT
        logger()->error()<<Q_FUNC_INFO<<" -> caught unkbnown error";
#else
        qWarning()<<Q_FUNC_INFO<<"caught unkbnown error";
#endif
        ErrUnknown status;
        return BaseResponse<InputModeModel>(status.getCode(), status.getMessage(), model);
    }

    NoError status;
    return BaseResponse<InputModeModel>(status.getCode(), status.getMessage(), model);
}

void OSDCMSInputMode::resetToPrevMode()
{
    //TODO: update repo
    //    repoInputMode->SetEntity(); //temp

    currentMode = previousMode;
//    if (lastUpdateMode == "position") {
//        currentMode.setPosition(previousMode.getPosition());
//    } else if (lastUpdateMode == "inertia"){
//        currentMode.setPosition(previousMode.getInersia());
//    }
}
