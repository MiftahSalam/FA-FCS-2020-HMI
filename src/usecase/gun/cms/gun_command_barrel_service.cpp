#include "gun_command_barrel_service.h"
#include "src/shared/common/errors/helper_err.h"
#include "src/shared/common/errors/err_json_parse.h"
#include "src/shared/common/errors/err_object_creation.h"
#include "src/shared/utils/utils.h"

GunCommandBarrelService* GunCommandBarrelService::instance = nullptr;

GunCommandBarrelService::GunCommandBarrelService(
        HttpClientWrapper *parent,
        GunCmsConfig *cmsConfig,
        GunCommandRepository *repoGunCmd
        ): HttpClientWrapper{parent}, cfgCms(cmsConfig), _repoGunCmd(repoGunCmd)
{
    if(parent == nullptr) {
        throw ErrObjectCreation();
    }
}

void GunCommandBarrelService::onReplyFinished()
{
    QByteArray respRaw = httpResponse->readAll();

    qDebug()<<Q_FUNC_INFO<<"respRaw: "<<respRaw;
    qDebug()<<Q_FUNC_INFO<<"err: "<<httpResponse->error();

    BaseResponse<GunCommandBarrelResponse> resp = errorResponse(httpResponse->error());
    if(resp.getHttpCode() != 0 || respRaw.isEmpty()) {
        emit signal_setBarrelResponse(resp);
        return;
    }

    resp = toResponse(respRaw);

    _repoGunCmd->SetBarrel(resp.getData().getAzimuth(), resp.getData().getElevation());

    emit signal_setBarrelResponse(resp);
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

void GunCommandBarrelService::setBarrel(GunCommandBarrelRequest request)
{
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

        qDebug()<<Q_FUNC_INFO<<"resp"<<resp.getHttpCode()<<resp.getMessage()<<resp.getData().getAzimuth()<<resp.getData().getElevation();

        return resp;
    } catch (ErrJsonParse &e) {
        qDebug()<<Q_FUNC_INFO<<"caught error: "<<e.getMessage();
    }  catch (...) {
        qDebug()<<Q_FUNC_INFO<<"caught unkbnown error";
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
        qDebug()<<Q_FUNC_INFO<<"caught error: "<<e.getMessage();
        return BaseResponse<GunCommandBarrelResponse>(e.getCode(), e.getMessage(), model);
    }  catch (...) {
        qDebug()<<Q_FUNC_INFO<<"caught unkbnown error";
        ErrUnknown status;
        return BaseResponse<GunCommandBarrelResponse>(status.getCode(), status.getMessage(), model);
    }

    NoError status;
    return BaseResponse<GunCommandBarrelResponse>(status.getCode(), status.getMessage(), model);

}
