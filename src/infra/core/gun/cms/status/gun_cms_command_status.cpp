#include "gun_cms_command_status.h"
#include "src/shared/common/errors/err_http.h"
#include "src/shared/common/errors/helper_err.h"
#include "src/shared/common/errors/err_object_creation.h"
#include "src/shared/utils/utils.h"

#ifdef USE_LOG4QT
#include <log4qt/logger.h>
LOG4QT_DECLARE_STATIC_LOGGER(logger, GunCmsCommandStatus)
#else
#include <QDebug>
#endif

const char * REQ_CTX_GUN_STATUS = "ctx-req";
const char * REQ_CTX_GUN_STATUS_NEED_CONFIRM_KEY = "ctx-req-need-confirm";

GunCmsCommandStatus* GunCmsCommandStatus::instance = nullptr;

GunCmsCommandStatus::GunCmsCommandStatus(
    HttpClientWrapper *parent,
    GunCmsConfig *cmsConfig): HttpClientWrapper{parent}, cfgCms(cmsConfig)
{
    if(parent == nullptr) {
        throw ErrObjectCreation();
    }
}

void GunCmsCommandStatus::onReplyFinished()
{
    QNetworkReply *objSender = dynamic_cast<QNetworkReply *>(sender());
    QByteArray respRaw = objSender->readAll();
    auto curReqCtx = objSender->property(REQ_CTX_GUN_STATUS).toMap();
    auto needConfirm = curReqCtx.value(REQ_CTX_GUN_STATUS_NEED_CONFIRM_KEY).toBool();

#ifdef USE_LOG4QT
    logger()->debug()<<Q_FUNC_INFO<<" -> respRaw: "<<respRaw;
    logger()->debug()<<Q_FUNC_INFO<<" -> err: "<<objSender->error();
#else
    qDebug()<<Q_FUNC_INFO<<"respRaw: "<<respRaw;
    qDebug()<<Q_FUNC_INFO<<"err: "<<objSender->error();
#endif

    BaseResponse<GunCommandStatusResponse> resp = toResponse(objSender->error(), respRaw);

    emit signal_setStatusResponse(resp, needConfirm);

    objSender->deleteLater();
}

GunCmsCommandStatus *GunCmsCommandStatus::getInstance(
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

        instance = new GunCmsCommandStatus(httpClient, cmsConfig);
    }

    return instance;
}

void GunCmsCommandStatus::set(GunCommandStatusRequest request)
{
    QNetworkRequest httpReq = QNetworkRequest(cfgCms->getInstance("")->getStatusUrl());
    httpReq.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    auto httpResponse = httpClient.post(httpReq, request.toJSON());
    connect(httpResponse, &QNetworkReply::finished, this, &GunCmsCommandStatus::onReplyFinished);
}

void GunCmsCommandStatus::setStatus(GunCommandStatusRequest request)
{
    set(request);
}

BaseResponse<GunCommandStatusResponse> GunCmsCommandStatus::toResponse(QNetworkReply::NetworkError err, QByteArray raw)
{
    GunCommandStatusResponse model(false, false, false, false, false, false);
    try {
        if (raw.isEmpty()) {
            throw ErrHttpConnRefused();
        }
        ErrHelper::throwHttpError(err);

        QJsonObject respObj = Utils::byteArrayToJsonObject(raw);
        int respCode = respObj["code"].toInt();
        QString respMsg = respObj["message"].toString();
        QJsonObject respData = respObj["data"].toObject();

        model = GunCommandStatusResponse::FromJsonObject(respData);
        model.setErr(NoError());

        BaseResponse<GunCommandStatusResponse> resp(respCode, respMsg, model);

#ifdef USE_LOG4QT
        logger()->debug()<<Q_FUNC_INFO<<" -> resp. http code: "<<resp.getHttpCode()
                        <<", message: "<<resp.getMessage()
                       <<", mount: "<<resp.getData().getMount()
                      <<", getSingleShot: "<<resp.getData().getSingleShot()
                     <<", getFireOrder: "<<resp.getData().getFireOrder()
                    <<", getProxFuze: "<<resp.getData().getProxFuze()
                   <<", getSiren: "<<resp.getData().getSiren()
                     ;
#else
        qDebug()<<Q_FUNC_INFO<<"resp"<<resp.getHttpCode()<<resp.getMessage()
               <<resp.getData().getMount()
              <<resp.getData().getSingleShot()
             <<resp.getData().getFireOrder()
            <<resp.getData().getProxFuze()
           <<resp.getData().getSiren()
             ;
#endif

        return resp;
    } catch (BaseError &e) {
#ifdef USE_LOG4QT
        logger()->error()<<Q_FUNC_INFO<<" -> caught error: "<<e.getMessage();
#else
        qWarning()<<Q_FUNC_INFO<<"caught error: "<<e.getMessage();
#endif
        model.setErr(e);

        return BaseResponse<GunCommandStatusResponse>(e.getCode(), e.getMessage(), model);
    }  catch (...) {
#ifdef USE_LOG4QT
        logger()->error()<<Q_FUNC_INFO<<" -> caught unkbnown error";
#else
        qWarning()<<Q_FUNC_INFO<<"caught unkbnown error";
#endif

        ErrUnknown status;
        model.setErr(status);

        return BaseResponse<GunCommandStatusResponse>(status.getCode(), status.getMessage(), model);
    }
}
