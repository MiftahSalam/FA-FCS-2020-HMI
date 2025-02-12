#include "gun_cms_command_barrel.h"
#include "src/shared/common/errors/err_http.h"
#include "src/shared/common/errors/helper_err.h"
#include "src/shared/common/errors/err_object_creation.h"
#include "src/shared/utils/utils.h"

#ifdef USE_LOG4QT
#include <log4qt/logger.h>
LOG4QT_DECLARE_STATIC_LOGGER(logger, GunCommandBarrelService)
#else
#include <QDebug>
#endif

const char * REQ_CTX_GUN_BARREL = "ctx-req";
const char * REQ_CTX_GUN_BARREL_NEED_CONFIRM_KEY = "ctx-req-need-confirm";

GunCmsCommandBarrel* GunCmsCommandBarrel::instance = nullptr;

GunCmsCommandBarrel::GunCmsCommandBarrel(
        HttpClientWrapper *parent,
        GunCmsConfig *cmsConfig
        ):
    HttpClientWrapper{parent},
    cfgCms(cmsConfig)
{
    if(parent == nullptr) {
        throw ErrObjectCreation();
    }
}

void GunCmsCommandBarrel::onReplyFinished()
{
    QNetworkReply *httpResponse = dynamic_cast<QNetworkReply *>(sender());
    QByteArray respRaw = httpResponse->readAll();
    auto curReqCtx = httpResponse->property(REQ_CTX_GUN_BARREL).toMap();
    auto needConfirm = curReqCtx.value(REQ_CTX_GUN_BARREL_NEED_CONFIRM_KEY).toBool();

#ifdef USE_LOG4QT
    logger()->debug()<<Q_FUNC_INFO<<" -> respRaw: "<<respRaw;
    logger()->debug()<<Q_FUNC_INFO<<" -> err: "<<httpResponse->error();
#else
    qDebug()<<Q_FUNC_INFO<<"respRaw: "<<respRaw;
    qDebug()<<Q_FUNC_INFO<<"err: "<<httpResponse->error();
#endif

    BaseResponse<GunCommandBarrelResponse> resp = toResponse(httpResponse->error(), respRaw);

    emit signal_setBarrelResponse(resp, needConfirm);

    httpResponse->deleteLater();
}

GunCmsCommandBarrel *GunCmsCommandBarrel::getInstance(
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

        instance = new GunCmsCommandBarrel(httpClient, cmsConfig);
    }

    return instance;
}

void GunCmsCommandBarrel::setBarrelWithConfirm(GunCommandBarrelRequest request, bool confirm)
{
    reqCtxObj.insert(REQ_CTX_GUN_BARREL_NEED_CONFIRM_KEY, confirm);

    set(request);
}

BaseResponse<GunCommandBarrelResponse> GunCmsCommandBarrel::toResponse(QNetworkReply::NetworkError err, QByteArray raw)
{
    GunCommandBarrelResponse model(0,0);
    try {
        if (raw.isEmpty()) {
            throw ErrHttpConnRefused();
        }
        ErrHelper::throwHttpError(err);

        QJsonObject respObj = Utils::byteArrayToJsonObject(raw);
        int respCode = respObj["code"].toInt();
        QString respMsg = respObj["message"].toString();
        QJsonObject respData = respObj["data"].toObject();

        model = GunCommandBarrelResponse::FromJsonObject(respData);
        model.setErr(NoError());

        BaseResponse<GunCommandBarrelResponse> resp(respCode, respMsg, model);

#ifdef USE_LOG4QT
        logger()->debug()<<Q_FUNC_INFO<<" -> resp. http code: "<<resp.getHttpCode()
                        <<", message: "<<resp.getMessage()
                       <<", az: "<<resp.getData().getAzimuth()
                      <<", el: "<<resp.getData().getElevation()
                        ;
#else
        qDebug()<<Q_FUNC_INFO<<"resp"<<resp.getHttpCode()<<resp.getMessage()<<resp.getData().getAzimuth()<<resp.getData().getElevation();
#endif

        return resp;
    } catch (BaseError &e) {
#ifdef USE_LOG4QT
        logger()->error()<<Q_FUNC_INFO<<" -> caught error: "<<e.getMessage();
#else
        qWarning()<<Q_FUNC_INFO<<"caught error: "<<e.getMessage();
#endif

        model.setErr(e);

        return BaseResponse<GunCommandBarrelResponse>(e.getCode(), e.getMessage(), model);
    }  catch (...) {
#ifdef USE_LOG4QT
        logger()->error()<<Q_FUNC_INFO<<" -> caught unkbnown error";
#else
        qWarning()<<Q_FUNC_INFO<<"caught unkbnown error";
#endif

        ErrUnknown status;
        model.setErr(status);

        return BaseResponse<GunCommandBarrelResponse>(status.getCode(), status.getMessage(), model);
    }
}

void GunCmsCommandBarrel::set(GunCommandBarrelRequest request)
{
    QNetworkRequest httpReq = QNetworkRequest(cfgCms->getInstance("")->getManualBarrelUrl());
    httpReq.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    auto httpResponse = httpClient.post(httpReq, request.toJSON());
    connect(httpResponse, &QNetworkReply::finished, this, &GunCmsCommandBarrel::onReplyFinished);
}
