#include "osd_cms_waterspeed_data.h"
#include "src/shared/common/errors/err_http.h"
#include "src/shared/common/errors/helper_err.h"
#include "src/shared/common/errors/err_object_creation.h"
#include "src/shared/utils/utils.h"

#ifdef USE_LOG4QT
#include <log4qt/logger.h>
LOG4QT_DECLARE_STATIC_LOGGER(logger, OSDCMSWaterSpeedData)
#else
#include <QDebug>
#endif

OSDCMSWaterSpeedData* OSDCMSWaterSpeedData::waterspeedData = nullptr;

OSDCMSWaterSpeedData::OSDCMSWaterSpeedData(
        HttpClientWrapper *parent,
        OSDCmsConfig *cmsConfig
        ): HttpClientWrapper(parent), cfgCms(cmsConfig)
{
    if(parent == nullptr) {
        throw ErrObjectCreation();
    }
}

OSDCMSWaterSpeedData *OSDCMSWaterSpeedData::getInstance(
    HttpClientWrapper *httpClient = nullptr,
    OSDCmsConfig *cmsConfig = nullptr)
{
    if (waterspeedData == nullptr) {
        if(cmsConfig == nullptr) {
            throw ErrObjectCreation();
        }

        if(httpClient == nullptr) {
            throw ErrObjectCreation();
        }

        waterspeedData = new OSDCMSWaterSpeedData(httpClient, cmsConfig);
    }
    return waterspeedData;
}

void OSDCMSWaterSpeedData::set(OSDSetWaterSpeedRequest request)
{
    QNetworkRequest httpReq = QNetworkRequest(cfgCms->getInstance("")->getManualDataUrl()+"/water-speed");
    httpReq.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    auto httpResponse = httpClient.put(httpReq, request.toJSON());
    connect(httpResponse, &QNetworkReply::finished, this, &OSDCMSWaterSpeedData::onReplyFinished);
}

void OSDCMSWaterSpeedData::onReplyFinished()
{
    QNetworkReply *httpResponse = dynamic_cast<QNetworkReply *>(sender());
    QByteArray respRaw = httpResponse->readAll();

#ifdef USE_LOG4QT
    logger()->debug()<<Q_FUNC_INFO<<" -> respRaw: "<<respRaw;
    logger()->debug()<<Q_FUNC_INFO<<" -> err: "<<httpResponse->error();
#else
    qDebug()<<Q_FUNC_INFO<<"respRaw: "<<respRaw;
    qDebug()<<Q_FUNC_INFO<<"err: "<<httpResponse->error();
#endif

    BaseResponse<WaterSpeedResponseModel> resp = toResponse(httpResponse->error(), respRaw);

    emit signal_setWaterSpeedResponse(resp);

    httpResponse->deleteLater();
}

BaseResponse<WaterSpeedResponseModel> OSDCMSWaterSpeedData::toResponse(QNetworkReply::NetworkError err, QByteArray raw)
{
    WaterSpeedResponseModel model(0, 0);
    try {
        if (raw.isEmpty()) {
            throw ErrHttpConnRefused();
        }
        ErrHelper::throwHttpError(err);

        QJsonObject respObj = Utils::byteArrayToJsonObject(raw);
        int respCode = respObj["code"].toInt();
        QString respMsg = respObj["message"].toString();
        QJsonObject respData = respObj["data"].toObject();
        WaterSpeedResponseModel model(respData["speed"].toDouble(),respData["course"].toDouble());

        model.setErr(NoError());
        model.setMode(OSD_MODE::MANUAL);
        model.setSource("manual");

        BaseResponse<WaterSpeedResponseModel> resp(respCode, respMsg, model);

#ifdef USE_LOG4QT
        logger()->debug()<<Q_FUNC_INFO<<" -> resp. http code: "<<resp.getHttpCode()
                        <<", message: "<<resp.getMessage()
                       <<", speed: "<<resp.getData().getSpeed()
                      <<", direction: "<<resp.getData().getCourse()
                        ;
#else
        qDebug()<<Q_FUNC_INFO<<"resp"<<resp.getHttpCode()<<resp.getMessage()<<resp.getData().getSpeed()<<resp.getData().getCourse();
#endif

        return resp;
    } catch (BaseError &e) {
#ifdef USE_LOG4QT
        logger()->error()<<Q_FUNC_INFO<<" -> caught error: "<<e.getMessage();
#else
        qWarning()<<Q_FUNC_INFO<<"caught error: "<<e.getMessage();
#endif

        model.setErr(e);
        model.setMode(OSD_MODE::MANUAL);
        model.setSource("manual");

        return BaseResponse<WaterSpeedResponseModel>(e.getCode(), e.getMessage(), model);
    }  catch (...) {
#ifdef USE_LOG4QT
        logger()->error()<<Q_FUNC_INFO<<" -> caught unkbnown error";
#else
        qWarning()<<Q_FUNC_INFO<<"caught unkbnown error";
#endif
        ErrUnknown status;
        return BaseResponse<WaterSpeedResponseModel>(status.getCode(), status.getMessage(), model);
    }
}
