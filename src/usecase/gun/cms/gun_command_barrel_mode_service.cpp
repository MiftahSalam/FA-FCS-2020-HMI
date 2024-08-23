#include "gun_command_barrel_mode_service.h"
#include "src/shared/common/errors/helper_err.h"
#include "src/shared/common/errors/err_json_parse.h"
#include "src/shared/common/errors/err_object_creation.h"
#include "src/shared/utils/utils.h"

GunCommandBarrelModeService* GunCommandBarrelModeService::instance = nullptr;

GunCommandBarrelModeService::GunCommandBarrelModeService(
        HttpClientWrapper *parent,
        GunCmsConfig *cmsConfig,
        GunCommandRepository *repoGunCmd
        ): HttpClientWrapper{parent}, cfgCms(cmsConfig), _repoGunCmd(repoGunCmd)
{
    synced = false;

    if(parent == nullptr) {
        throw ErrObjectCreation();
    }

    previousMode = repoGunCmd->GetBarrelMode()->getManualMode();

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &GunCommandBarrelModeService::onTimerTimeout);

    timer->start(1000);
}

void GunCommandBarrelModeService::onReplyFinished()
{
    QByteArray respRaw = httpResponse->readAll();

    qDebug()<<Q_FUNC_INFO<<"respRaw: "<<respRaw;
    qDebug()<<Q_FUNC_INFO<<"err: "<<httpResponse->error();

    BaseResponse<GunModeBarrelResponse> resp = errorResponse(httpResponse->error());
    if(resp.getHttpCode() != 0) {
        _repoGunCmd->SetBarrelMode(GunBarrelModeEntity(previousMode));
        synced = false;

        emit signal_setModeResponse(resp, !requestSync);
        return;
    }

    synced = true;
    resp = toResponse(respRaw);

    _repoGunCmd->SetBarrelMode(resp.getData().getManualMode());
    previousMode = _repoGunCmd->GetBarrelMode()->getManualMode();

    emit signal_setModeResponse(resp, !requestSync);
}

void GunCommandBarrelModeService::onTimerTimeout()
{
    sync();
}

GunCommandBarrelModeService *GunCommandBarrelModeService::getInstance(
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

        instance = new GunCommandBarrelModeService(httpClient, cmsConfig, repoGunCmd);
    }

    return instance;
}

void GunCommandBarrelModeService::setMode(bool manual)
{
    requestSync = false;
    _repoGunCmd->SetBarrelMode(GunBarrelModeEntity(manual));
    sendMode(GunModeBarrelRequest(_repoGunCmd->GetBarrelMode()->getManualMode()));
}

void GunCommandBarrelModeService::sendMode(GunModeBarrelRequest request)
{
    qDebug()<<Q_FUNC_INFO<<"gun barrel mode url: "<<cfgCms->getInstance("")->getModeUrl();

    QNetworkRequest httpReq = QNetworkRequest(cfgCms->getInstance("")->getModeUrl());
    httpReq.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    httpResponse = httpClient.post(httpReq, request.toJSON());
    connect(httpResponse, &QNetworkReply::finished, this, &GunCommandBarrelModeService::onReplyFinished);
}

BaseResponse<GunModeBarrelResponse> GunCommandBarrelModeService::toResponse(QByteArray raw)
{
    try {
        QJsonObject respObj = Utils::byteArrayToJsonObject(raw);
        int respCode = respObj["code"].toInt();
        QString respMsg = respObj["message"].toString();
        QJsonObject respData = respObj["data"].toObject();
        GunModeBarrelResponse model(respData["manual_mode"].toBool());
        BaseResponse<GunModeBarrelResponse> resp(respCode, respMsg, model);

        qDebug()<<Q_FUNC_INFO<<"resp"<<resp.getHttpCode()<<resp.getMessage()<<resp.getData().getManualMode();

        return resp;
    } catch (ErrJsonParse &e) {
        qDebug()<<Q_FUNC_INFO<<"caught error: "<<e.getMessage();
    }  catch (...) {
        qDebug()<<Q_FUNC_INFO<<"caught unkbnown error";
    }

    ErrUnknown status;
    GunModeBarrelResponse model(true);
    return BaseResponse<GunModeBarrelResponse>(status.getCode(), status.getMessage(), model);

}

BaseResponse<GunModeBarrelResponse> GunCommandBarrelModeService::errorResponse(QNetworkReply::NetworkError err)
{
    GunModeBarrelResponse model(true);
    try {
        ErrHelper::throwHttpError(err);
    } catch (BaseError &e) {
        qDebug()<<Q_FUNC_INFO<<"caught error: "<<e.getMessage();
        return BaseResponse<GunModeBarrelResponse>(e.getCode(), e.getMessage(), model);
    }  catch (...) {
        qDebug()<<Q_FUNC_INFO<<"caught unkbnown error";
        ErrUnknown status;
        return BaseResponse<GunModeBarrelResponse>(status.getCode(), status.getMessage(), model);
    }

    NoError status;
    return BaseResponse<GunModeBarrelResponse>(status.getCode(), status.getMessage(), model);

}

void GunCommandBarrelModeService::sync()
{
    if(!synced) {
        qDebug()<<Q_FUNC_INFO<<"syncing";

        requestSync = true;
        sendMode(GunModeBarrelRequest(_repoGunCmd->GetBarrelMode()->getManualMode()));
    }
}
