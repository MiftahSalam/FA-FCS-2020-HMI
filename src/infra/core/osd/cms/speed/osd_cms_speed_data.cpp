#include "osd_cms_speed_data.h"
#include "src/shared/common/errors/err_http.h"
#include "src/shared/common/errors/helper_err.h"
#include "src/shared/common/errors/err_object_creation.h"
#include "src/shared/utils/utils.h"

#ifdef USE_LOG4QT
#include <log4qt/logger.h>
LOG4QT_DECLARE_STATIC_LOGGER(logger, OSDCMSSpeedData)
#else
#include <QDebug>
#endif

OSDCMSSpeedData* OSDCMSSpeedData::speedData = nullptr;

OSDCMSSpeedData::OSDCMSSpeedData(
    HttpClientWrapper *parent,
    OSDCmsConfig *cmsConfig): HttpClientWrapper(parent), cfgCms(cmsConfig)
{
    if(parent == nullptr) {
        throw ErrObjectCreation();
    }
}

OSDCMSSpeedData *OSDCMSSpeedData::getInstance(
    HttpClientWrapper *httpClient = nullptr,
    OSDCmsConfig *cmsConfig = nullptr)
{
    if (speedData == nullptr) {
        if(cmsConfig == nullptr) {
            throw ErrObjectCreation();
        }

        if(httpClient == nullptr) {
            throw ErrObjectCreation();
        }

        speedData = new OSDCMSSpeedData(httpClient, cmsConfig);
    }

    return speedData;
}

void OSDCMSSpeedData::set(OSDSetSpeedRequest request)
{
    QNetworkRequest httpReq = QNetworkRequest(cfgCms->getInstance("")->getManualDataUrl()+"/speed");
    httpReq.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    auto httpResponse = httpClient.put(httpReq, request.toJSON());
    connect(httpResponse, &QNetworkReply::finished, this, &OSDCMSSpeedData::onReplyFinished);
}

void OSDCMSSpeedData::onReplyFinished()
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

    BaseResponse<SpeedModel> resp = toResponse(httpResponse->error(), respRaw);

    emit signal_setSpeedResponse(resp);

    httpResponse->deleteLater();
}

BaseResponse<SpeedModel> OSDCMSSpeedData::toResponse(QNetworkReply::NetworkError err, QByteArray raw)
{
    SpeedModel model(0, 0);
    try {
        if (raw.isEmpty()) {
            throw ErrHttpConnRefused();
        }
        ErrHelper::throwHttpError(err);

        QJsonObject respObj = Utils::byteArrayToJsonObject(raw);
        int respCode = respObj["code"].toInt();
        QString respMsg = respObj["message"].toString();
        QJsonObject respData = respObj["data"].toObject();
        SpeedModel model(respData["sog"].toDouble(),respData["cog"].toDouble());

        model.setErr(NoError());
        model.setMode(OSD_MODE::MANUAL);
        model.setSource("manual");

        BaseResponse<SpeedModel> resp(respCode, respMsg, model);

#ifdef USE_LOG4QT
        logger()->debug()<<Q_FUNC_INFO<<" -> resp. http code: "<<resp.getHttpCode()
                        <<", message: "<<resp.getMessage()
                       <<", sog: "<<resp.getData().getSpeed()
                      <<", cog: "<<resp.getData().getCourse()
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

        return BaseResponse<SpeedModel>(e.getCode(), e.getMessage(), model);
    }  catch (...) {
#ifdef USE_LOG4QT
        logger()->error()<<Q_FUNC_INFO<<" -> caught unkbnown error";
#else
        qWarning()<<Q_FUNC_INFO<<"caught unkbnown error";
#endif
        ErrUnknown status;
        return BaseResponse<SpeedModel>(status.getCode(), status.getMessage(), model);
    }
}
