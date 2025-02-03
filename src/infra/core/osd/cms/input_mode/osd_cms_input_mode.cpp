#include "osd_cms_input_mode.h"
#include "src/shared/common/errors/helper_err.h"
#include "src/shared/common/errors/err_http.h"
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
    cfgCms(cmsConfig)
{
    reqCtxObj.insert(REQ_CTX_NEED_CONFIRM_KEY, true);
    reqCtxObj.insert(REQ_CTX_LAST_UPDATE_DATA_KEY, "");

    if(parent == nullptr) {
        throw ErrObjectCreation();
    }
}

OSDCMSInputMode *OSDCMSInputMode::getInstance(
    HttpClientWrapper *httpClient = nullptr,
    OSDCmsConfig *cmsConfig = nullptr
    )
{
    if (inputMode == nullptr) {
        if(cmsConfig == nullptr) {
            throw ErrObjectCreation();
        }

        if(httpClient == nullptr) {
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

void OSDCMSInputMode::setDataMode(const bool confirm, const QString &dataFisis, const OSDInputModeRequest modeReq)
{
    reqCtxObj.insert(REQ_CTX_NEED_CONFIRM_KEY, confirm);
    reqCtxObj.insert(REQ_CTX_LAST_UPDATE_DATA_KEY, dataFisis);

    set(modeReq);
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

    BaseResponse<InputModeResponseModel> resp = toResponse(objSender->error(), respRaw);

    emit signal_setModeResponse(lastUpdateData, resp, needConfirm);

    objSender->deleteLater();
}

BaseResponse<InputModeResponseModel> OSDCMSInputMode::toResponse(QNetworkReply::NetworkError err, QByteArray raw)
{
    InputModeResponseModel model(false, false, false, false, false, false);
    try {
        if (raw.isEmpty()) {
            throw ErrHttpConnRefused();
        }
        ErrHelper::throwHttpError(err);

        QJsonObject respObj = Utils::byteArrayToJsonObject(raw);
        int respCode = respObj["code"].toInt();
        QString respMsg = respObj["message"].toString();
        QJsonObject respData = respObj["data"].toObject();
        InputModeResponseModel model(
            respData["position"].toBool(),
            respData["speed"].toBool(),
            respData["inertia"].toBool(),
            respData["water_speed"].toBool(),
            respData["wind"].toBool(),
            respData["weather"].toBool()
            );
        BaseResponse<InputModeResponseModel> resp(respCode, respMsg, model);

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
    } catch (BaseError &e) {
#ifdef USE_LOG4QT
        logger()->error()<<Q_FUNC_INFO<<" -> caught error: "<<e.getMessage();
#else
        qWarning()<<Q_FUNC_INFO<<"caught error: "<<e.getMessage();
#endif
        return BaseResponse<InputModeResponseModel>(e.getCode(), e.getMessage(), model);
    }  catch (...) {
#ifdef USE_LOG4QT
        logger()->error()<<Q_FUNC_INFO<<" -> caught unkbnown error";
#else
        qWarning()<<Q_FUNC_INFO<<"caught unkbnown error";
#endif
        ErrUnknown status;
        return BaseResponse<InputModeResponseModel>(status.getCode(), status.getMessage(), model);
    }
}
