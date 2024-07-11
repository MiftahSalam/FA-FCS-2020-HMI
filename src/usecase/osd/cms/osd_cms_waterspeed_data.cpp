#include "osd_cms_waterspeed_data.h"
#include "src/shared/common/errors/helper_err.h"
#include "src/shared/common/errors/err_json_parse.h"
#include "src/shared/common/errors/err_object_creation.h"
#include "src/shared/utils/utils.h"

OSDCMSWaterSpeedData* OSDCMSWaterSpeedData::waterspeedData = nullptr;

OSDCMSWaterSpeedData::OSDCMSWaterSpeedData(
        HttpClientWrapper *parent,
        OSDCmsConfig *cmsConfig,
        OSDWaterSpeedRepository *repoWS
        ): HttpClientWrapper(parent), cfgCms(cmsConfig), repoWS(repoWS)
{
    if(parent == nullptr) {
        throw ErrObjectCreation();
    }
}

OSDCMSWaterSpeedData *OSDCMSWaterSpeedData::getInstance(
        HttpClientWrapper *httpClient = nullptr,
        OSDCmsConfig *cmsConfig = nullptr,
        OSDWaterSpeedRepository *repoWS
        )
{
    if (waterspeedData == nullptr) {
        if(cmsConfig == nullptr) {
            throw ErrObjectCreation();
        }

        if(httpClient == nullptr) {
            throw ErrObjectCreation();
        }

        if(repoWS == nullptr) {
            throw ErrObjectCreation();
        }
        waterspeedData = new OSDCMSWaterSpeedData(httpClient, cmsConfig, repoWS);
    }
    return waterspeedData;
}

void OSDCMSWaterSpeedData::set(OSDSetWaterSpeedRequest request)
{
    QNetworkRequest httpReq = QNetworkRequest(cfgCms->getInstance("")->getManualDataUrl()+"/waterspeed");
    httpReq.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    httpResponse = httpClient.put(httpReq, request.toJSON());
    connect(httpResponse, &QNetworkReply::finished, this, &OSDCMSWaterSpeedData::onReplyFinished);
}

void OSDCMSWaterSpeedData::onReplyFinished()
{
    QByteArray respRaw = httpResponse->readAll();

    qDebug()<<Q_FUNC_INFO<<"respRaw: "<<respRaw;
    qDebug()<<Q_FUNC_INFO<<"err: "<<httpResponse->error();

    BaseResponse<WaterSpeedModel> resp = errorResponse(httpResponse->error());
    if(resp.getHttpCode() != 0) {
        emit signal_setWaterSpeedResponse(resp);
        return;
    }

    resp = toResponse(respRaw);

    //TODO: update repo
    repoWS->SetWaterSpeed(OSDWaterSpeedEntity(
                             resp.getData().getSpeed(),
                             resp.getData().getCourse(),
                             "manual",
                             "",
                             OSD_MODE::MANUAL //temp
                             ));

    emit signal_setWaterSpeedResponse(resp);
}

BaseResponse<WaterSpeedModel> OSDCMSWaterSpeedData::toResponse(QByteArray raw)
{
    try {
        QJsonObject respObj = Utils::byteArrayToJsonObject(raw);
        int respCode = respObj["code"].toInt();
        QString respMsg = respObj["message"].toString();
        QJsonObject respData = respObj["data"].toObject();
        WaterSpeedModel model(respData["waterspeed"].toDouble(),respData["watercourse"].toDouble());
        BaseResponse<WaterSpeedModel> resp(respCode, respMsg, model);

        qDebug()<<Q_FUNC_INFO<<"resp"<<resp.getHttpCode()<<resp.getMessage()<<resp.getData().getSpeed()<<resp.getData().getCourse();

        return resp;
    } catch (ErrJsonParse &e) {
        qDebug()<<Q_FUNC_INFO<<"caught error: "<<e.getMessage();
    }  catch (...) {
        qDebug()<<Q_FUNC_INFO<<"caught unkbnown error";
    }

    ErrUnknown status;
    WaterSpeedModel model(0, 0);
    return BaseResponse<WaterSpeedModel>(status.getCode(), status.getMessage(), model);
}

BaseResponse<WaterSpeedModel> OSDCMSWaterSpeedData::errorResponse(QNetworkReply::NetworkError err)
{
    WaterSpeedModel model(0, 0);
    try {
        ErrHelper::throwHttpError(err);
    } catch (BaseError &e) {
        qDebug()<<Q_FUNC_INFO<<"caught error: "<<e.getMessage();
        return BaseResponse<WaterSpeedModel>(e.getCode(), e.getMessage(), model);
    }  catch (...) {
        qDebug()<<Q_FUNC_INFO<<"caught unkbnown error";
        ErrUnknown status;
        return BaseResponse<WaterSpeedModel>(status.getCode(), status.getMessage(), model);
    }

    NoError status;
    return BaseResponse<WaterSpeedModel>(status.getCode(), status.getMessage(), model);
}
