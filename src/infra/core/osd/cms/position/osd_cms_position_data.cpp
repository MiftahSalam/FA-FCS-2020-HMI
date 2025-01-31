#include "osd_cms_position_data.h"
#include "src/shared/common/errors/err_http.h"
#include "src/shared/common/errors/helper_err.h"
#include "src/shared/common/errors/err_object_creation.h"
#include "src/shared/utils/utils.h"

#ifdef USE_LOG4QT
#include <log4qt/logger.h>
LOG4QT_DECLARE_STATIC_LOGGER(logger, OSDCMSPositionData)
#else
#include <QDebug>
#endif

OSDCMSPositionData* OSDCMSPositionData::positionData = nullptr;

OSDCMSPositionData::OSDCMSPositionData(
        HttpClientWrapper *parent,
        OSDCmsConfig *cmsConfig
        ): HttpClientWrapper(parent), cfgCms(cmsConfig)
{
    if(parent == nullptr) {
        throw ErrObjectCreation();
    }
}

OSDCMSPositionData *OSDCMSPositionData::getInstance(
        HttpClientWrapper *httpClient = nullptr,
        OSDCmsConfig *cmsConfig = nullptr
        )
{
    if (positionData == nullptr) {
        if(cmsConfig == nullptr) {
            throw ErrObjectCreation();
        }

        if(httpClient == nullptr) {
            throw ErrObjectCreation();
        }

        positionData = new OSDCMSPositionData(httpClient, cmsConfig);
    }

    return positionData;
}

void OSDCMSPositionData::set(OSDSetPositionRequest request)
{
    QNetworkRequest httpReq = QNetworkRequest(cfgCms->getInstance("")->getManualDataUrl()+"/position");
    httpReq.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    auto httpResponse = httpClient.put(httpReq, request.toJSON());
    connect(httpResponse, &QNetworkReply::finished, this, &OSDCMSPositionData::onReplyFinished);
}

void OSDCMSPositionData::onReplyFinished()
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

    BaseResponse<PositionModel> resp = toResponse(httpResponse->error(), respRaw);

    emit signal_setPositionResponse(resp);

    httpResponse->deleteLater();
}

BaseResponse<PositionModel> OSDCMSPositionData::toResponse(QNetworkReply::NetworkError err, QByteArray raw)
{
    PositionModel model(-91, -181);

    try {
        if (raw.isEmpty()) {
            throw ErrHttpConnRefused();
        }
        ErrHelper::throwHttpError(err);

        QJsonObject respObj = Utils::byteArrayToJsonObject(raw);
        int respCode = respObj["code"].toInt();
        QString respMsg = respObj["message"].toString();
        QJsonObject respData = respObj["data"].toObject();
        PositionModel model(respData["latitude"].toDouble(-91),respData["longitude"].toDouble(-181));

        model.setErr(NoError());
        model.setMode(OSD_MODE::MANUAL);
        model.setSource("manual");

        BaseResponse<PositionModel> resp(respCode, respMsg, model);

#ifdef USE_LOG4QT
        logger()->debug()<<Q_FUNC_INFO<<" -> resp. http code: "<<resp.getHttpCode()
                        <<", message: "<<resp.getMessage()
                       <<", latitude: "<<resp.getData().getLatitude()
                      <<", longitude: "<<resp.getData().getLongitude()
                        ;
#else
        qDebug()<<Q_FUNC_INFO<<"resp"<<resp.getHttpCode()<<resp.getMessage()<<resp.getData().getLatitude()<<resp.getData().getLongitude();
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

        return BaseResponse<PositionModel>(e.getCode(), e.getMessage(), model);
    }  catch (...) {
#ifdef USE_LOG4QT
        logger()->error()<<Q_FUNC_INFO<<" -> caught unkbnown error";
#else
        qWarning()<<Q_FUNC_INFO<<"caught unkbnown error";
#endif
        ErrUnknown status;
        return BaseResponse<PositionModel>(status.getCode(), status.getMessage(), model);
    }    
}
