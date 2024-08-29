#include "gun_command_status_service.h"
#include "src/shared/common/errors/helper_err.h"
#include "src/shared/common/errors/err_json_parse.h"
#include "src/shared/common/errors/err_object_creation.h"
#include "src/shared/utils/utils.h"

GunCommandStatusService* GunCommandStatusService::instance = nullptr;

GunCommandStatusService::GunCommandStatusService(
        HttpClientWrapper *parent,
        GunCmsConfig *cmsConfig,
        GunCommandRepository *repoGunCmd
        ): HttpClientWrapper{parent}, cfgCms(cmsConfig), _repoGunCmd(repoGunCmd)
{
    if(parent == nullptr) {
        throw ErrObjectCreation();
    }
}

void GunCommandStatusService::onReplyFinished()
{
    QByteArray respRaw = httpResponse->readAll();

    qDebug()<<Q_FUNC_INFO<<"respRaw: "<<respRaw;
    qDebug()<<Q_FUNC_INFO<<"err: "<<httpResponse->error();

    BaseResponse<GunCommandStatusResponse> resp = errorResponse(httpResponse->error());
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
}

GunCommandStatusService *GunCommandStatusService::getInstance(
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

        instance = new GunCommandStatusService(httpClient, cmsConfig, repoGunCmd);
    }

    return instance;
}

void GunCommandStatusService::setStatus(GunCommandStatusRequest request)
{
    _repoGunCmd->SetStatus(GunStatusCommandEntity(
                               request.getMount(),
                               request.getSingleShot(),
                               request.getFireOrder(),
                               request.getProxFuze(),
                               request.getSiren()
                               ));
    sendStatus(request);
}

void GunCommandStatusService::sendStatus(GunCommandStatusRequest request)
{
    QNetworkRequest httpReq = QNetworkRequest(cfgCms->getInstance("")->getStatusUrl());
    httpReq.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    httpResponse = httpClient.post(httpReq, request.toJSON());
    connect(httpResponse, &QNetworkReply::finished, this, &GunCommandStatusService::onReplyFinished);
}

void GunCommandStatusService::setStatusMount(bool on)
{
    _repoGunCmd->SetMount(on);
    auto curStatus = _repoGunCmd->GetStatus();
    sendStatus(GunCommandStatusRequest(
                   curStatus->mount(),
                   curStatus->single_shot(),
                   curStatus->fire(),
                   curStatus->proximity(),
                   curStatus->siren()
                   ));
}

void GunCommandStatusService::setStatusSingleShot(bool on)
{
    _repoGunCmd->SetSingleShot(on);
    auto curStatus = _repoGunCmd->GetStatus();
    sendStatus(GunCommandStatusRequest(
                   curStatus->mount(),
                   curStatus->single_shot(),
                   curStatus->fire(),
                   curStatus->proximity(),
                   curStatus->siren()
                   ));
}

void GunCommandStatusService::setStatusFire(bool on)
{
    _repoGunCmd->SetFire(on);
    auto curStatus = _repoGunCmd->GetStatus();
    sendStatus(GunCommandStatusRequest(
                   curStatus->mount(),
                   curStatus->single_shot(),
                   curStatus->fire(),
                   curStatus->proximity(),
                   curStatus->siren()
                   ));
}

void GunCommandStatusService::setStatusProxFuze(bool on)
{
    _repoGunCmd->SetProximity(on);
    auto curStatus = _repoGunCmd->GetStatus();
    sendStatus(GunCommandStatusRequest(
                   curStatus->mount(),
                   curStatus->single_shot(),
                   curStatus->fire(),
                   curStatus->proximity(),
                   curStatus->siren()
                   ));
}

void GunCommandStatusService::setStatusSiren(bool on)
{
    _repoGunCmd->SetSiren(on);
    auto curStatus = _repoGunCmd->GetStatus();
    sendStatus(GunCommandStatusRequest(
                   curStatus->mount(),
                   curStatus->single_shot(),
                   curStatus->fire(),
                   curStatus->proximity(),
                   curStatus->siren()
                   ));
}

BaseResponse<GunCommandStatusResponse> GunCommandStatusService::toResponse(QByteArray raw)
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

        qDebug()<<Q_FUNC_INFO<<"resp"<<resp.getHttpCode()<<resp.getMessage()
               <<resp.getData().getMount()
              <<resp.getData().getSingleShot()
             <<resp.getData().getFireOrder()
            <<resp.getData().getProxFuze()
           <<resp.getData().getSiren()
             ;

        return resp;
    } catch (ErrJsonParse &e) {
        qDebug()<<Q_FUNC_INFO<<"caught error: "<<e.getMessage();
    }  catch (...) {
        qDebug()<<Q_FUNC_INFO<<"caught unkbnown error";
    }

    ErrUnknown status;
    GunCommandStatusResponse model;
    return BaseResponse<GunCommandStatusResponse>(status.getCode(), status.getMessage(), model);

}

BaseResponse<GunCommandStatusResponse> GunCommandStatusService::errorResponse(QNetworkReply::NetworkError err)
{
    GunCommandStatusResponse model;
    try {
        ErrHelper::throwHttpError(err);
    } catch (BaseError &e) {
        qDebug()<<Q_FUNC_INFO<<"caught error: "<<e.getMessage();
        return BaseResponse<GunCommandStatusResponse>(e.getCode(), e.getMessage(), model);
    }  catch (...) {
        qDebug()<<Q_FUNC_INFO<<"caught unkbnown error";
        ErrUnknown status;
        return BaseResponse<GunCommandStatusResponse>(status.getCode(), status.getMessage(), model);
    }

    NoError status;
    return BaseResponse<GunCommandStatusResponse>(status.getCode(), status.getMessage(), model);

}
