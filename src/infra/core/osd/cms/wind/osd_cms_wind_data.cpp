#include "osd_cms_wind_data.h"
#include "src/shared/common/errors/err_http.h"
#include "src/shared/common/errors/helper_err.h"
#include "src/shared/common/errors/err_object_creation.h"
#include "src/shared/utils/utils.h"

#ifdef USE_LOG4QT
#include <log4qt/logger.h>
LOG4QT_DECLARE_STATIC_LOGGER(logger, OSDCMSWindData)
#else
#include <QDebug>
#endif

OSDCMSWindData* OSDCMSWindData::windData = nullptr;

OSDCMSWindData::OSDCMSWindData(
        HttpClientWrapper *parent,
        OSDCmsConfig *cmsConfig
        ): HttpClientWrapper(parent), cfgCms(cmsConfig)
{
    if(parent == nullptr) {
        throw ErrObjectCreation();
    }
}

OSDCMSWindData *OSDCMSWindData::getInstance(
        HttpClientWrapper *httpClient = nullptr,
        OSDCmsConfig *cmsConfig = nullptr
        )
{
    if (windData == nullptr) {
        if(cmsConfig == nullptr) {
            throw ErrObjectCreation();
        }

        if(httpClient == nullptr) {
            throw ErrObjectCreation();
        }

        windData = new OSDCMSWindData(httpClient, cmsConfig);
    }
    return windData;
}

void OSDCMSWindData::set(OSDSetWindRequest request)
{
    QNetworkRequest httpReq = QNetworkRequest(cfgCms->getInstance("")->getManualDataUrl()+"/wind");
    httpReq.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    auto httpResponse = httpClient.put(httpReq, request.toJSON());
    connect(httpResponse, &QNetworkReply::finished, this, &OSDCMSWindData::onReplyFinished);
}

void OSDCMSWindData::onReplyFinished()
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

    BaseResponse<WindResponseModel> resp = toResponse(httpResponse->error(), respRaw);

    emit signal_setWindResponse(resp);

    httpResponse->deleteLater();
}

BaseResponse<WindResponseModel> OSDCMSWindData::toResponse(QNetworkReply::NetworkError err, QByteArray raw)
{
    WindResponseModel model(0, 0);
    try {
        if (raw.isEmpty()) {
            throw ErrHttpConnRefused();
        }
        ErrHelper::throwHttpError(err);

        QJsonObject respObj = Utils::byteArrayToJsonObject(raw);
        int respCode = respObj["code"].toInt();
        QString respMsg = respObj["message"].toString();
        QJsonObject respData = respObj["data"].toObject();
        WindResponseModel model(respData["speed"].toDouble(),respData["direction"].toDouble());

        model.setErr(NoError());
        model.setMode(OSD_MODE::MANUAL);
        model.setSource("manual");

        BaseResponse<WindResponseModel> resp(respCode, respMsg, model);

#ifdef USE_LOG4QT
        logger()->debug()<<Q_FUNC_INFO<<" -> resp. http code: "<<resp.getHttpCode()
                        <<", message: "<<resp.getMessage()
                       <<", speed: "<<resp.getData().getSpeed()
                      <<", dir: "<<resp.getData().getDirection()
                        ;
#else
        qDebug()<<Q_FUNC_INFO<<"resp"<<resp.getHttpCode()<<resp.getMessage()<<resp.getData().getSpeed()<<resp.getData().getDirection();
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

        return BaseResponse<WindResponseModel>(e.getCode(), e.getMessage(), model);
    }  catch (...) {
#ifdef USE_LOG4QT
        logger()->error()<<Q_FUNC_INFO<<" -> caught unkbnown error";
#else
        qWarning()<<Q_FUNC_INFO<<"caught unkbnown error";
#endif

        ErrUnknown status;
        return BaseResponse<WindResponseModel>(status.getCode(), status.getMessage(), model);
    }
}
