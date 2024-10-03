#include "gun_command_barrel_service.h"
#include "src/shared/common/errors/helper_err.h"
#include "src/shared/common/errors/err_json_parse.h"
#include "src/shared/common/errors/err_object_creation.h"
#include "src/shared/utils/utils.h"

#ifdef USE_LOG4QT
#include <log4qt/logger.h>
LOG4QT_DECLARE_STATIC_LOGGER(logger, GunCommandBarrelService)
#else
#include <QDebug>
#endif

GunCommandBarrelService* GunCommandBarrelService::instance = nullptr;

GunCommandBarrelService::GunCommandBarrelService(
        HttpClientWrapper *parent,
        GunCmsConfig *cmsConfig,
        GunCommandRepository *repoGunCmd
        ):
    HttpClientWrapper{parent},
    cfgCms(cmsConfig),
    _repoGunCmd(repoGunCmd),
    latestConfirm(false)
{
    if(parent == nullptr) {
        throw ErrObjectCreation();
    }
}

void GunCommandBarrelService::onReplyFinished()
{
    QByteArray respRaw = httpResponse->readAll();

#ifdef USE_LOG4QT
    logger()->debug()<<Q_FUNC_INFO<<" -> respRaw: "<<respRaw;
    logger()->debug()<<Q_FUNC_INFO<<" -> err: "<<httpResponse->error();
#else
    qDebug()<<Q_FUNC_INFO<<"respRaw: "<<respRaw;
    qDebug()<<Q_FUNC_INFO<<"err: "<<httpResponse->error();
#endif

    BaseResponse<GunCommandBarrelResponse> resp = errorResponse(httpResponse->error());
    if(resp.getHttpCode() != 0 || respRaw.isEmpty()) {
        emit signal_setBarrelResponse(resp, latestConfirm);
        return;
    }

    resp = toResponse(respRaw);

    _repoGunCmd->SetBarrel(resp.getData().getAzimuth(), resp.getData().getElevation());

    emit signal_setBarrelResponse(resp, latestConfirm);
}

GunCommandBarrelService *GunCommandBarrelService::getInstance(
        HttpClientWrapper *httpClient = nullptr,
        GunCmsConfig *cmsConfig,
        GunCommandRepository *repoGunCmd
        )
{
    if (instance == nullptr) {
        if(cmsConfig == nullptr) {
            throw ErrObjectCreation();
        }

        if(httpClient == nullptr) {
            throw ErrObjectCreation();
        }

        if(repoGunCmd == nullptr) {
            throw ErrObjectCreation();
        }

        instance = new GunCommandBarrelService(httpClient, cmsConfig, repoGunCmd);
    }

    return instance;
}

void GunCommandBarrelService::setBarrelWithConfirm(GunCommandBarrelRequest request, bool confirm)
{
    latestConfirm = confirm;

    QNetworkRequest httpReq = QNetworkRequest(cfgCms->getInstance("")->getManualBarrelUrl());
    httpReq.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    httpResponse = httpClient.post(httpReq, request.toJSON());
    connect(httpResponse, &QNetworkReply::finished, this, &GunCommandBarrelService::onReplyFinished);
}

BaseResponse<GunCommandBarrelResponse> GunCommandBarrelService::toResponse(QByteArray raw)
{
    try {
        QJsonObject respObj = Utils::byteArrayToJsonObject(raw);
        int respCode = respObj["code"].toInt();
        QString respMsg = respObj["message"].toString();
        QJsonObject respData = respObj["data"].toObject();
        GunCommandBarrelResponse model(respData["azimuth"].toDouble(),respData["elevation"].toDouble());
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
    GunCommandBarrelResponse model(-1000, 9000);
    return BaseResponse<GunCommandBarrelResponse>(status.getCode(), status.getMessage(), model);

}

BaseResponse<GunCommandBarrelResponse> GunCommandBarrelService::errorResponse(QNetworkReply::NetworkError err)
{
    GunCommandBarrelResponse model(-1000, 9000);
    try {
        ErrHelper::throwHttpError(err);
    } catch (BaseError &e) {
#ifdef USE_LOG4QT
        logger()->error()<<Q_FUNC_INFO<<" -> caught error: "<<e.getMessage();
#else
        qWarning()<<Q_FUNC_INFO<<"caught error: "<<e.getMessage();
#endif
        return BaseResponse<GunCommandBarrelResponse>(e.getCode(), e.getMessage(), model);
    }  catch (...) {
#ifdef USE_LOG4QT
        logger()->error()<<Q_FUNC_INFO<<" -> caught unkbnown error";
#else
        qWarning()<<Q_FUNC_INFO<<"caught unkbnown error";
#endif
        ErrUnknown status;
        return BaseResponse<GunCommandBarrelResponse>(status.getCode(), status.getMessage(), model);
    }

    NoError status;
    return BaseResponse<GunCommandBarrelResponse>(status.getCode(), status.getMessage(), model);

}
