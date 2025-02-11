#include "gun_cms_command_status.h"
#include "src/shared/common/errors/helper_err.h"
#include "src/shared/common/errors/err_json_parse.h"
#include "src/shared/common/errors/err_object_creation.h"
#include "src/shared/utils/utils.h"

#ifdef USE_LOG4QT
#include <log4qt/logger.h>
LOG4QT_DECLARE_STATIC_LOGGER(logger, GunCmsCommandStatus)
#else
#include <QDebug>
#endif

GunCmsCommandStatus* GunCmsCommandStatus::instance = nullptr;

GunCmsCommandStatus::GunCmsCommandStatus(
        HttpClientWrapper *parent,
        GunCmsConfig *cmsConfig,
        GunCommandRepository *repoGunCmd
        ): HttpClientWrapper{parent}, cfgCms(cmsConfig), _repoGunCmd(repoGunCmd)
{
    if(parent == nullptr) {
        throw ErrObjectCreation();
    }
}

void GunCmsCommandStatus::onReplyFinished()
{
    QNetworkReply *objSender = dynamic_cast<QNetworkReply *>(sender());
    QByteArray respRaw = objSender->readAll();

#ifdef USE_LOG4QT
    logger()->debug()<<Q_FUNC_INFO<<" -> respRaw: "<<respRaw;
    logger()->debug()<<Q_FUNC_INFO<<" -> err: "<<objSender->error();
#else
    qDebug()<<Q_FUNC_INFO<<"respRaw: "<<respRaw;
    qDebug()<<Q_FUNC_INFO<<"err: "<<objSender->error();
#endif

    BaseResponse<GunCommandStatusResponse> resp = errorResponse(objSender->error());
    if(resp.getHttpCode() != 0 || respRaw.isEmpty()) {
        emit signal_setStatusResponse(resp);
        return;
    }

    resp = toResponse(respRaw);

    _repoGunCmd->SetStatus(GunStatusCommandEntity(
                               resp.getData().getMount(),
                               resp.getData().getSingleShot(),
                               resp.getData().getFireOrder(),
                               resp.getData().getProxFuze(),
                               resp.getData().getSiren()
                               ));

    emit signal_setStatusResponse(resp);

    objSender->deleteLater();
}

GunCmsCommandStatus *GunCmsCommandStatus::getInstance(
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

        instance = new GunCmsCommandStatus(httpClient, cmsConfig, repoGunCmd);
    }

    return instance;
}

const GunStatusCommandEntity *GunCmsCommandStatus::getCurrentStatus() const
{
    return _repoGunCmd->GetStatus();
}

void GunCmsCommandStatus::setStatus(GunCommandStatusRequest request)
{
    /*
    _repoGunCmd->SetStatus(GunStatusCommandEntity(
                               request.getMount(),
                               request.getSingleShot(),
                               request.getFireOrder(),
                               request.getProxFuze(),
                               request.getSiren()
                               ));
    */
    sendStatus(request);
}

void GunCmsCommandStatus::sendStatus(GunCommandStatusRequest request)
{
    QNetworkRequest httpReq = QNetworkRequest(cfgCms->getInstance("")->getStatusUrl());
    httpReq.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    auto httpResponse = httpClient.post(httpReq, request.toJSON());
    connect(httpResponse, &QNetworkReply::finished, this, &GunCmsCommandStatus::onReplyFinished);
}

void GunCmsCommandStatus::setStatusMount(bool on)
{
    //    _repoGunCmd->SetMount(on);
    auto curStatus = _repoGunCmd->GetStatus();
    sendStatus(GunCommandStatusRequest(
                   on,
                   curStatus->single_shot(),
                   curStatus->fire(),
                   curStatus->proximity(),
                   curStatus->siren()
                   ));
}

void GunCmsCommandStatus::setStatusSingleShot(bool on)
{
    //    _repoGunCmd->SetSingleShot(on);
    auto curStatus = _repoGunCmd->GetStatus();
    sendStatus(GunCommandStatusRequest(
                   curStatus->mount(),
                   on,
                   curStatus->fire(),
                   curStatus->proximity(),
                   curStatus->siren()
                   ));
}

void GunCmsCommandStatus::setStatusFire(bool on)
{
    //    _repoGunCmd->SetFire(on);
    auto curStatus = _repoGunCmd->GetStatus();
    sendStatus(GunCommandStatusRequest(
                   curStatus->mount(),
                   curStatus->single_shot(),
                   on,
                   curStatus->proximity(),
                   curStatus->siren()
                   ));
}

void GunCmsCommandStatus::setStatusProxFuze(bool on)
{
    //    _repoGunCmd->SetProximity(on);
    auto curStatus = _repoGunCmd->GetStatus();
    sendStatus(GunCommandStatusRequest(
                   curStatus->mount(),
                   curStatus->single_shot(),
                   curStatus->fire(),
                   on,
                   curStatus->siren()
                   ));
}

void GunCmsCommandStatus::setStatusSiren(bool on)
{
    //    _repoGunCmd->SetSiren(on);
    auto curStatus = _repoGunCmd->GetStatus();
    sendStatus(GunCommandStatusRequest(
                   curStatus->mount(),
                   curStatus->single_shot(),
                   curStatus->fire(),
                   curStatus->proximity(),
                   on
                   ));
}

BaseResponse<GunCommandStatusResponse> GunCmsCommandStatus::toResponse(QByteArray raw)
{
    try {
        QJsonObject respObj = Utils::byteArrayToJsonObject(raw);
        int respCode = respObj["code"].toInt();
        QString respMsg = respObj["message"].toString();
        QJsonObject respData = respObj["data"].toObject();
        GunCommandStatusResponse model(
                    respData["mount"].toBool(),
                respData["single_shot"].toBool(),
                respData["fire_order"].toBool(),
                respData["not_fire_order"].toBool(),
                respData["paralizing_proximity_fuze"].toBool(),
                respData["siren_button"].toBool()
                );
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
    GunCommandStatusResponse model;
    return BaseResponse<GunCommandStatusResponse>(status.getCode(), status.getMessage(), model);

}

BaseResponse<GunCommandStatusResponse> GunCmsCommandStatus::errorResponse(QNetworkReply::NetworkError err)
{
    GunCommandStatusResponse model;
    try {
        ErrHelper::throwHttpError(err);
    } catch (BaseError &e) {
#ifdef USE_LOG4QT
        logger()->error()<<Q_FUNC_INFO<<" -> caught error: "<<e.getMessage();
#else
        qWarning()<<Q_FUNC_INFO<<"caught error: "<<e.getMessage();
#endif
        return BaseResponse<GunCommandStatusResponse>(e.getCode(), e.getMessage(), model);
    }  catch (...) {
#ifdef USE_LOG4QT
        logger()->error()<<Q_FUNC_INFO<<" -> caught unkbnown error";
#else
        qWarning()<<Q_FUNC_INFO<<"caught unkbnown error";
#endif
        ErrUnknown status;
        return BaseResponse<GunCommandStatusResponse>(status.getCode(), status.getMessage(), model);
    }

    NoError status;
    return BaseResponse<GunCommandStatusResponse>(status.getCode(), status.getMessage(), model);

}
