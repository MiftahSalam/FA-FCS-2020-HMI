#include "engagement_data_correction_40mm_service.h"
#include "src/shared/common/errors/helper_err.h"
#include "src/shared/common/errors/err_json_parse.h"
#include "src/shared/common/errors/err_object_creation.h"
#include "src/shared/utils/utils.h"

EngagementDataCorrection40mmService* EngagementDataCorrection40mmService::instance = nullptr;

EngagementDataCorrection40mmService::EngagementDataCorrection40mmService(
    HttpClientWrapper *parent,
    TrackWeaponAssignConfig *cmsConfig
    ): HttpClientWrapper{parent}, cfgCms(cmsConfig)
{
    if(parent == nullptr) {
        throw ErrObjectCreation();
    }
}

void EngagementDataCorrection40mmService::onReplyFinished()
{
    QByteArray respRaw = httpResponse->readAll();

    qDebug()<<Q_FUNC_INFO<<"respRaw: "<<respRaw;
    qDebug()<<Q_FUNC_INFO<<"err: "<<httpResponse->error();

    BaseResponse<EngagementCorrectionSetResponse> resp = errorResponse(httpResponse->error());
    if(resp.getHttpCode() != 0 || respRaw.isEmpty()) {
        emit signal_setCorrectionResponse(resp);
        return;
    }

    resp = toResponse(respRaw);

    emit signal_setCorrectionResponse(resp);
}

BaseResponse<EngagementCorrectionSetResponse> EngagementDataCorrection40mmService::toResponse(QByteArray raw)
{
    try {
        QJsonObject respObj = Utils::byteArrayToJsonObject(raw);
        int respCode = respObj["code"].toInt();
        QString respMsg = respObj["message"].toString();
        QJsonObject respData = respObj["data"].toObject();
        EngagementCorrectionSetResponse model(respData["azimuth_correction"].toDouble(),respData["elevation_correction"].toDouble());
        BaseResponse<EngagementCorrectionSetResponse> resp(respCode, respMsg, model);

        qDebug()<<Q_FUNC_INFO<<"resp Correction-------->>>"<<resp.getHttpCode()<<resp.getMessage()<<resp.getData().getAzimuthCorrection()<<resp.getData().getElevationCorrection();

        return resp;
    } catch (ErrJsonParse &e) {
        qDebug()<<Q_FUNC_INFO<<"caught error: "<<e.getMessage();
    }  catch (...) {
        qDebug()<<Q_FUNC_INFO<<"caught unkbnown error";
    }

    ErrUnknown status;
    EngagementCorrectionSetResponse model(-1000, 9000);
    return BaseResponse<EngagementCorrectionSetResponse>(status.getCode(), status.getMessage(), model);
}

BaseResponse<EngagementCorrectionSetResponse> EngagementDataCorrection40mmService::errorResponse(QNetworkReply::NetworkError err)
{
    EngagementCorrectionSetResponse model(-1000, 9000);
    try {
        ErrHelper::throwHttpError(err);
    } catch (BaseError &e) {
        qDebug()<<Q_FUNC_INFO<<"caught error: "<<e.getMessage();
        return BaseResponse<EngagementCorrectionSetResponse>(e.getCode(), e.getMessage(), model);
    }  catch (...) {
        qDebug()<<Q_FUNC_INFO<<"caught unknown error";
        ErrUnknown status;
        return BaseResponse<EngagementCorrectionSetResponse>(status.getCode(), status.getMessage(), model);
    }

    NoError status;
    return BaseResponse<EngagementCorrectionSetResponse>(status.getCode(), status.getMessage(), model);
}

EngagementDataCorrection40mmService *EngagementDataCorrection40mmService::getInstance(
    HttpClientWrapper *httpClient = nullptr,
    TrackWeaponAssignConfig *cmsConfig
    )
{
    if (instance == nullptr) {
        if(cmsConfig == nullptr){
            throw ErrObjectCreation();
        }

        if(httpClient == nullptr) {
            throw ErrObjectCreation();
        }

        instance = new EngagementDataCorrection40mmService(httpClient, cmsConfig);
    }
    return instance;
}

void EngagementDataCorrection40mmService::setCorrection(EngagementCorrectionSetRequest request)
{
    QNetworkRequest httpReq = QNetworkRequest(cfgCms->getInstance("")->getEngageCorrectionUrl());
    httpReq.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    httpResponse = httpClient.post(httpReq, request.toJSON());
    connect(httpResponse, &QNetworkReply::finished, this, &EngagementDataCorrection40mmService::onReplyFinished);
}
