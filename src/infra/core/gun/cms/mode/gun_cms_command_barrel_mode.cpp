#include "gun_cms_command_barrel_mode.h"
#include "src/shared/common/errors/helper_err.h"
#include "src/shared/common/errors/err_http.h"
#include "src/shared/common/errors/err_object_creation.h"
#include "src/shared/utils/utils.h"

#ifdef USE_LOG4QT
#include <log4qt/logger.h>
LOG4QT_DECLARE_STATIC_LOGGER(logger, GunCmsCommandBarrelMode)
#else
#include <QDebug>
#endif

const char * REQ_CTX_GUN_BARREL_MODE = "ctx-req";
const char * REQ_CTX_GUN_BARREL_MODE_NEED_CONFIRM_KEY = "ctx-req-need-confirm";

GunCmsCommandBarrelMode* GunCmsCommandBarrelMode::instance = nullptr;

GunCmsCommandBarrelMode::GunCmsCommandBarrelMode(
    HttpClientWrapper *parent, GunCmsConfig *cmsConfig
    ): HttpClientWrapper{parent}, cfgCms(cmsConfig)
{    
    reqCtxObj.insert(REQ_CTX_GUN_BARREL_MODE_NEED_CONFIRM_KEY, true);

    if(parent == nullptr) {
        throw ErrObjectCreation();
    }
}

void GunCmsCommandBarrelMode::onReplyFinished()
{
    QNetworkReply *httpResponse = dynamic_cast<QNetworkReply *>(sender());
    QByteArray respRaw = httpResponse->readAll();
    auto curReqCtx = httpResponse->property(REQ_CTX_GUN_BARREL_MODE).toMap();
    auto needConfirm = curReqCtx.value(REQ_CTX_GUN_BARREL_MODE_NEED_CONFIRM_KEY).toBool();

#ifdef USE_LOG4QT
    logger()->debug()<<Q_FUNC_INFO<<" -> respRaw: "<<respRaw;
    logger()->debug()<<Q_FUNC_INFO<<" -> err: "<<httpResponse->error();
    logger()->debug()<<Q_FUNC_INFO<<" -> resp prop needConfirm: "<<needConfirm;
#else
    qDebug()<<Q_FUNC_INFO<<"respRaw: "<<respRaw;
    qDebug()<<Q_FUNC_INFO<<"err: "<<httpResponse->error();
    qDebug()<<Q_FUNC_INFO<<"resp prop needConfirm: "<<needConfirm;
    qDebug()<<Q_FUNC_INFO<<"resp prop lastUpdateData: "<<lastUpdateData;
#endif
    
    BaseResponse<GunModeBarrelResponse> resp = toResponse(httpResponse->error(), respRaw);

    emit signal_setModeResponse(resp, needConfirm);

    httpResponse->deleteLater();
}

GunCmsCommandBarrelMode *GunCmsCommandBarrelMode::getInstance(
    HttpClientWrapper *httpClient = nullptr,
    GunCmsConfig *cmsConfig)
{
    if (instance == nullptr) {
        if(cmsConfig == nullptr) {
            throw ErrObjectCreation();
        }

        if(httpClient == nullptr) {
            throw ErrObjectCreation();
        }

        instance = new GunCmsCommandBarrelMode(httpClient, cmsConfig);
    }

    return instance;
}

void GunCmsCommandBarrelMode::sendMode(const bool confirm, GunModeBarrelRequest request)
{
    reqCtxObj.insert(REQ_CTX_GUN_BARREL_MODE_NEED_CONFIRM_KEY, confirm);

    set(request);
}

void GunCmsCommandBarrelMode::set(GunModeBarrelRequest request)
{
#ifdef USE_LOG4QT
    logger()->debug()<<Q_FUNC_INFO<<" -> gun barrel mode url: "<<cfgCms->getInstance("")->getModeUrl();
#else
    qDebug()<<Q_FUNC_INFO<<"gun barrel mode url: "<<cfgCms->getInstance("")->getModeUrl();
#endif

    QNetworkRequest httpReq = QNetworkRequest(cfgCms->getInstance("")->getModeUrl());
    httpReq.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    auto httpResponse = httpClient.post(httpReq, request.toJSON());
    httpResponse->setProperty(REQ_CTX_GUN_BARREL_MODE, reqCtxObj);
    connect(httpResponse, &QNetworkReply::finished, this, &GunCmsCommandBarrelMode::onReplyFinished);
}

BaseResponse<GunModeBarrelResponse> GunCmsCommandBarrelMode::toResponse(QNetworkReply::NetworkError err, QByteArray raw)
{
    GunModeBarrelResponse model(true);
    try {
        if (raw.isEmpty()) {
            throw ErrHttpConnRefused();
        }
        ErrHelper::throwHttpError(err);

        QJsonObject respObj = Utils::byteArrayToJsonObject(raw);
        int respCode = respObj["code"].toInt();
        QString respMsg = respObj["message"].toString();
        QJsonObject respData = respObj["data"].toObject();
        GunModeBarrelResponse model(respData["manual_mode"].toBool());
        BaseResponse<GunModeBarrelResponse> resp(respCode, respMsg, model);

#ifdef USE_LOG4QT
        logger()->debug()<<Q_FUNC_INFO<<" -> resp. http code: "<<resp.getHttpCode()
                        <<", message: "<<resp.getMessage()
                       <<", mode: "<<resp.getData().getManualMode();
#else
        qDebug()<<Q_FUNC_INFO<<"resp"<<resp.getHttpCode()<<resp.getMessage()<<resp.getData().getManualMode();
#endif

        return resp;
    } catch (BaseError &e) {
#ifdef USE_LOG4QT
        logger()->error()<<Q_FUNC_INFO<<" -> caught error: "<<e.getMessage();
#else
        qWarning()<<Q_FUNC_INFO<<"caught error: "<<e.getMessage();
#endif
        return BaseResponse<GunModeBarrelResponse>(e.getCode(), e.getMessage(), model);
    }  catch (...) {
#ifdef USE_LOG4QT
        logger()->error()<<Q_FUNC_INFO<<" -> caught unkbnown error";
#else
        qWarning()<<Q_FUNC_INFO<<"caught unkbnown error";
#endif
        ErrUnknown status;
        return BaseResponse<GunModeBarrelResponse>(status.getCode(), status.getMessage(), model);
    }
}
