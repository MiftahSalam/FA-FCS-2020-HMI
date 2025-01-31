#include "osd_cms_gyro_data.h"
#include "src/shared/common/errors/err_http.h"
#include "src/shared/common/errors/helper_err.h"
#include "src/shared/common/errors/err_object_creation.h"
#include "src/shared/utils/utils.h"

#ifdef USE_LOG4QT
#include <log4qt/logger.h>
LOG4QT_DECLARE_STATIC_LOGGER(logger, OSDCMSGyroData)
#else
#include <QDebug>
#endif

OSDCMSGyroData* OSDCMSGyroData::gyroData = nullptr;

OSDCMSGyroData::OSDCMSGyroData(
    HttpClientWrapper *parent,
    OSDCmsConfig *cmsConfig): HttpClientWrapper(parent), cfgCms(cmsConfig)
{
    if(parent == nullptr) {
        throw ErrObjectCreation();
    }
}

OSDCMSGyroData *OSDCMSGyroData::getInstance(
    HttpClientWrapper *httpClient = nullptr,
    OSDCmsConfig *cmsConfig = nullptr)
{
    if (gyroData == nullptr) {
        if(cmsConfig == nullptr) {
            throw ErrObjectCreation();
        }

        if(httpClient == nullptr) {
            throw ErrObjectCreation();
        }

        gyroData = new OSDCMSGyroData(httpClient, cmsConfig);
    }

    return gyroData;

}

void OSDCMSGyroData::set(OSDSetGyroRequest request)
{
    QNetworkRequest httpReq = QNetworkRequest(cfgCms->getInstance("")->getManualDataUrl()+"/inertia");
    httpReq.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    auto httpResponse = httpClient.put(httpReq, request.toJSON());
    connect(httpResponse, &QNetworkReply::finished, this, &OSDCMSGyroData::onReplyFinished);

}

void OSDCMSGyroData::onReplyFinished()
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

    BaseResponse<GyroModel> resp = toResponse(httpResponse->error(), respRaw);

    emit signal_setGyroResponse(resp);

    httpResponse->deleteLater();
}

BaseResponse<GyroModel> OSDCMSGyroData::toResponse(QNetworkReply::NetworkError err, QByteArray raw)
{
    GyroModel model(-1, 90, 90);
    try {
        if (raw.isEmpty()) {
            throw ErrHttpConnRefused();
        }
        ErrHelper::throwHttpError(err);

        QJsonObject respObj = Utils::byteArrayToJsonObject(raw);
        int respCode = respObj["code"].toInt();
        QString respMsg = respObj["message"].toString();
        QJsonObject respData = respObj["data"].toObject();
        GyroModel model(respData["heading"].toDouble(),respData["pitch"].toDouble(),respData["roll"].toDouble());

        model.setErr(NoError());
        model.setMode(OSD_MODE::MANUAL);
        model.setSource("manual");

        BaseResponse<GyroModel> resp(respCode, respMsg, model);

#ifdef USE_LOG4QT
        logger()->debug()<<Q_FUNC_INFO<<" -> resp. http code: "<<resp.getHttpCode()
                          <<", message: "<<resp.getMessage()
                          <<", heading: "<<resp.getData().getHeading()
                          <<", pitch: "<<resp.getData().getPicth()
                          <<", roll: "<<resp.getData().getRoll()
            ;
#else
        qDebug()<<Q_FUNC_INFO<<"resp"<<resp.getHttpCode()<<resp.getMessage()<<resp.getData().getHeading()<<resp.getData().getPicth()<<resp.getData().getRoll();
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

        return BaseResponse<GyroModel>(e.getCode(), e.getMessage(), model);
    }  catch (...) {
#ifdef USE_LOG4QT
        logger()->error()<<Q_FUNC_INFO<<" -> caught unkbnown error";
#else
        qWarning()<<Q_FUNC_INFO<<"caught unkbnown error";
#endif

        ErrUnknown status;
        return BaseResponse<GyroModel>(status.getCode(), status.getMessage(), model);
    }
}
