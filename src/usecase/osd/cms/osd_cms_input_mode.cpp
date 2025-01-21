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

const char * REQ_CTX_OSD_MODE = "ctx-req";
const char * REQ_CTX_NEED_CONFIRM_KEY = "ctx-req-need-confirm";
const char * REQ_CTX_LAST_UPDATE_DATA_KEY = "ctx-req-lastUpdate";

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

    reqCtxObj.insert(REQ_CTX_NEED_CONFIRM_KEY, true);
    reqCtxObj.insert(REQ_CTX_LAST_UPDATE_DATA_KEY, "");

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

    auto curHttpResponse = httpClient.put(httpReq, request.toJSON());
    curHttpResponse->setProperty(REQ_CTX_OSD_MODE, reqCtxObj);
    connect(curHttpResponse, &QNetworkReply::finished, this, &OSDCMSInputMode::onReplyFinished);
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

    reqCtxObj.insert(REQ_CTX_NEED_CONFIRM_KEY, true);
    reqCtxObj.insert(REQ_CTX_LAST_UPDATE_DATA_KEY, dataFisis);

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
        reqCtxObj.insert(REQ_CTX_NEED_CONFIRM_KEY, false);
        reqCtxObj.insert(REQ_CTX_LAST_UPDATE_DATA_KEY, "");

        set(currentMode);
    }
}

void OSDCMSInputMode::onReplyFinished()
{
    QNetworkReply *objSender = dynamic_cast<QNetworkReply *>(sender());
    QByteArray respRaw = objSender->readAll();
    auto respErr = objSender->error();
    auto curReqCtx = objSender->property(REQ_CTX_OSD_MODE).toMap();
    auto lastUpdateData = curReqCtx.value(REQ_CTX_LAST_UPDATE_DATA_KEY).toString();
    auto needConfirm = curReqCtx.value(REQ_CTX_NEED_CONFIRM_KEY).toBool();

#ifdef USE_LOG4QT
    logger()->debug()<<Q_FUNC_INFO<<" -> respRaw: "<<respRaw;
    logger()->debug()<<Q_FUNC_INFO<<" -> err: "<<(int)respErr;
    logger()->debug()<<Q_FUNC_INFO<<" -> resp prop needConfirm: "<<needConfirm;
    logger()->debug()<<Q_FUNC_INFO<<" -> resp prop lastUpdateData: "<<lastUpdateData;
#else
    qDebug()<<Q_FUNC_INFO<<"respRaw: "<<respRaw;
    qDebug()<<Q_FUNC_INFO<<"err: "<<respErr;
    qDebug()<<Q_FUNC_INFO<<"resp prop needConfirm: "<<needConfirm;
    qDebug()<<Q_FUNC_INFO<<"resp prop lastUpdateData: "<<lastUpdateData;
#endif

    BaseResponse<InputModeModel> resp = errorResponse(objSender->error());
    if(resp.getHttpCode() != 0 || respRaw.isEmpty()) {
        resetToPrevMode();
        synced = false;

        emit signal_setModeResponse(lastUpdateData, resp, needConfirm);
        // emit signal_setModeResponse(lastUpdateMode, resp, needConfirm);

        return;
    }

    synced = true;
    resp = toResponse(respRaw);

    previousMode = currentMode;
    //TODO: update repo
    //    repoInputMode->SetEntity(); //temp

    emit signal_setModeResponse(lastUpdateData, resp, needConfirm);
    // emit signal_setModeResponse(lastUpdateMode, resp, needConfirm);

    objSender->deleteLater();
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
        qDebug()<<Q_FUNC_INFO<<"resp"<<resp.getHttpCode()<<resp.getMessage()<<resp.getData().getInersia();
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
}
