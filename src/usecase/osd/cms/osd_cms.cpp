#include <QNetworkRequest>
#include <QtDebug>

#include "src/shared/common/errors/helper_err.h"
#include "src/shared/common/errors/err_json_parse.h"
#include "src/shared/common/errors/err_object_creation.h"
#include "src/shared/utils/utils.h"
#include "osd_cms.h"


OSDCMS::OSDCMS(QObject *parent, OSDCmsConfig *cmsConfig): QObject(parent), cfgCms(cmsConfig)
{
    if(cmsConfig == nullptr) {
        throw ErrObjectCreation();
    }

    serviceOSDCMSPosition = new OSDCMSPositionData(new HttpClientWrapper(), cmsConfig);
}

OSDCMSPositionData *OSDCMS::getServiceOSDCMSPosition() const
{
    return serviceOSDCMSPosition;
}

OSDCMSPositionData::OSDCMSPositionData(HttpClientWrapper *parent, OSDCmsConfig *cmsConfig): HttpClientWrapper(parent), cfgCms(cmsConfig)
{
    if(cmsConfig == nullptr) {
        throw ErrObjectCreation();
    }
}

void OSDCMSPositionData::setPosition(OSDSetPositionRequest request)
{
    QNetworkRequest httpReq = QNetworkRequest(cfgCms->getInstance("")->getManualDataUrl()+"/position");
    httpReq.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    httpResponse = httpClient.put(httpReq, request.toJSON());
    connect(httpResponse, &QNetworkReply::finished, this, &OSDCMSPositionData::onReplyFinished);
}

void OSDCMSPositionData::onReplyFinished()
{
    QByteArray respRaw = httpResponse->readAll();

    qDebug()<<Q_FUNC_INFO<<"respRaw: "<<respRaw;
    qDebug()<<Q_FUNC_INFO<<"err: "<<httpResponse->error();

    BaseResponse<PositionModel> respErr = errorResponse(httpResponse->error());
    if(respErr.getHttpCode() != 0) emit signal_setPositionResponse(respErr);

    BaseResponse<PositionModel> resp = toResponse(respRaw);

    emit signal_setPositionResponse(resp);
}

BaseResponse<PositionModel> OSDCMSPositionData::toResponse(QByteArray raw)
{
    try {
        QJsonObject respObj = Utils::byteArrayToJsonObject(raw);
        int respCode = respObj["code"].toInt();
        QString respMsg = respObj["message"].toString();
        QJsonObject respData = respObj["data"].toObject();
        PositionModel model(respData["latitude"].toDouble(-91),respData["longitude"].toDouble(-181));
        BaseResponse<PositionModel> resp(respCode, respMsg, &model);

        qDebug()<<Q_FUNC_INFO<<"resp"<<resp.getHttpCode()<<resp.getMessage()<<resp.getData()->getLatitude()<<resp.getData()->getLongitude();

        return resp;
    } catch (ErrJsonParse &e) {
        qDebug()<<Q_FUNC_INFO<<"caught error: "<<e.getMessage();
    }  catch (...) {
        qDebug()<<Q_FUNC_INFO<<"caught unkbnown error";
    }

    return BaseResponse<PositionModel>(ERROR_CODE_UNKNOWN.first, ERROR_CODE_UNKNOWN.second, nullptr);
}

BaseResponse<PositionModel> OSDCMSPositionData::errorResponse(QNetworkReply::NetworkError err)
{
    try {
        ErrHelper::throwHttpError(err);
    } catch (BaseError &e) {
        qDebug()<<Q_FUNC_INFO<<"caught error: "<<e.getMessage();
        return BaseResponse<PositionModel>(e.getCode(), e.getMessage(), nullptr);
    }  catch (...) {
        qDebug()<<Q_FUNC_INFO<<"caught unkbnown error";
        return BaseResponse<PositionModel>(ERROR_CODE_UNKNOWN.first, ERROR_CODE_UNKNOWN.second, nullptr);
    }

    NoError status;
    return BaseResponse<PositionModel>(status.getCode(), status.getMessage(), nullptr);
}

