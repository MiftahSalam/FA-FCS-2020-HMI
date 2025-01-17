#include "engagement_data_correction_40mm_service.h"
#include "src/shared/common/errors/helper_err.h"
#include "src/shared/common/errors/err_json_parse.h"
#include "src/shared/common/errors/err_object_creation.h"
#include "src/shared/utils/utils.h"

#ifdef USE_LOG4QT
#include <log4qt/logger.h>
LOG4QT_DECLARE_STATIC_LOGGER(logger, EngagementDataCorrection40mmService)
#else
#include <QDebug>
#endif

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
    QNetworkReply *objSender = dynamic_cast<QNetworkReply *>(sender());
    QByteArray respRaw = objSender->readAll();

#ifdef USE_LOG4QT
    logger()->debug()<<Q_FUNC_INFO<<" -> respRaw: "<<respRaw;
    logger()->debug()<<Q_FUNC_INFO<<" -> err: "<<objSender->error();
#else
    qDebug()<<Q_FUNC_INFO<<"respRaw:"<<respRaw;
    qDebug()<<Q_FUNC_INFO<<"err:"<<objSender->error();
#endif

    BaseResponse<EngagementCorrectionSetResponse> resp = errorResponse(objSender->error());
    if(resp.getHttpCode() != 0 || respRaw.isEmpty()) {
        emit signal_setCorrectionResponse(resp);
        return;
    }

    resp = toResponse(respRaw);

    emit signal_setCorrectionResponse(resp);

    objSender->deleteLater();
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

#ifdef USE_LOG4QT
        logger()->debug()<<Q_FUNC_INFO<<" -> resp Correction. http code: "<<resp.getHttpCode()
                        <<", message: "<<resp.getMessage()
                       <<", azimuth: "<<resp.getData().getAzimuthCorrection()
                      <<", elevation: "<<resp.getData().getAzimuthCorrection();
#else
        qDebug()<<Q_FUNC_INFO<<"resp Correction-------->>>"<<resp.getHttpCode()<<resp.getMessage()<<resp.getData().getAzimuthCorrection()<<resp.getData().getElevationCorrection();
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
    EngagementCorrectionSetResponse model(-1000, 9000);
    return BaseResponse<EngagementCorrectionSetResponse>(status.getCode(), status.getMessage(), model);
}

BaseResponse<EngagementCorrectionSetResponse> EngagementDataCorrection40mmService::errorResponse(QNetworkReply::NetworkError err)
{
    EngagementCorrectionSetResponse model(-1000, 9000);
    try {
        ErrHelper::throwHttpError(err);
    } catch (BaseError &e) {
#ifdef USE_LOG4QT
        logger()->error()<<Q_FUNC_INFO<<" -> caught error: "<<e.getMessage();
#else
        qWarning()<<Q_FUNC_INFO<<"caught error: "<<e.getMessage();
#endif
        return BaseResponse<EngagementCorrectionSetResponse>(e.getCode(), e.getMessage(), model);
    }  catch (...) {
#ifdef USE_LOG4QT
        logger()->error()<<Q_FUNC_INFO<<" -> caught unkbnown error";
#else
        qWarning()<<Q_FUNC_INFO<<"caught unkbnown error";
#endif
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

    auto curHttpResponse = httpClient.post(httpReq, request.toJSON());
    connect(curHttpResponse, &QNetworkReply::finished, this, &EngagementDataCorrection40mmService::onReplyFinished);
}
