#include "osd_cms_speed_data.h"
#include "src/shared/common/errors/helper_err.h"
#include "src/shared/common/errors/err_json_parse.h"
#include "src/shared/common/errors/err_object_creation.h"
#include "src/shared/utils/utils.h"

OSDCMSSpeedData* OSDCMSSpeedData::speedData = nullptr;


OSDCMSSpeedData::OSDCMSSpeedData(
        HttpClientWrapper *parent,
        OSDCmsConfig *cmsConfig,
        OSDSpeedRepository *repoSpeed
        ): HttpClientWrapper(parent), cfgCms(cmsConfig), repoSpeed(repoSpeed)
{
    if(parent == nullptr) {
        throw ErrObjectCreation();
    }
}

OSDCMSSpeedData *OSDCMSSpeedData::getInstance(
        HttpClientWrapper *httpClient = nullptr,
        OSDCmsConfig *cmsConfig = nullptr,
        OSDSpeedRepository *repoSpeed
        )
{
    if (speedData == nullptr) {
        if(cmsConfig == nullptr) {
            throw ErrObjectCreation();
        }

        if(httpClient == nullptr) {
            throw ErrObjectCreation();
        }

        if(repoSpeed == nullptr) {
            throw ErrObjectCreation();
        }

        speedData = new OSDCMSSpeedData(httpClient, cmsConfig, repoSpeed);
    }

    return speedData;
}

void OSDCMSSpeedData::set(OSDSetSpeedRequest request)
{
    QNetworkRequest httpReq = QNetworkRequest(cfgCms->getInstance("")->getManualDataUrl()+"/speed");
    httpReq.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    httpResponse = httpClient.put(httpReq, request.toJSON());
    connect(httpResponse, &QNetworkReply::finished, this, &OSDCMSSpeedData::onReplyFinished);
}

void OSDCMSSpeedData::onReplyFinished()
{
    QByteArray respRaw = httpResponse->readAll();

    qDebug()<<Q_FUNC_INFO<<"respRaw: "<<respRaw;
    qDebug()<<Q_FUNC_INFO<<"err: "<<httpResponse->error();

    BaseResponse<SpeedModel> resp = errorResponse(httpResponse->error());
    if(resp.getHttpCode() != 0) {
        emit signal_setSpeedResponse(resp);
        return;
    }

    resp = toResponse(respRaw);

    repoSpeed->SetSpeed(OSDSpeedEntity(
                             resp.getData().getSpeed(),
                             resp.getData().getCourse(),
                             "manual",
                             "",
                             OSD_MODE::MANUAL //temp
                             ));

    emit signal_setSpeedResponse(resp);
}

BaseResponse<SpeedModel> OSDCMSSpeedData::toResponse(QByteArray raw)
{
    try {
        QJsonObject respObj = Utils::byteArrayToJsonObject(raw);
        int respCode = respObj["code"].toInt();
        QString respMsg = respObj["message"].toString();
        QJsonObject respData = respObj["data"].toObject();
        SpeedModel model(respData["sog"].toDouble(),respData["cog"].toDouble());
        BaseResponse<SpeedModel> resp(respCode, respMsg, model);

        qDebug()<<Q_FUNC_INFO<<"resp"<<resp.getHttpCode()<<resp.getMessage()<<resp.getData().getSpeed()<<resp.getData().getCourse();

        return resp;
    } catch (ErrJsonParse &e) {
        qDebug()<<Q_FUNC_INFO<<"caught error: "<<e.getMessage();
    }  catch (...) {
        qDebug()<<Q_FUNC_INFO<<"caught unkbnown error";
    }

    ErrUnknown status;
    SpeedModel model(0, 0);
    return BaseResponse<SpeedModel>(status.getCode(), status.getMessage(), model);

}

BaseResponse<SpeedModel> OSDCMSSpeedData::errorResponse(QNetworkReply::NetworkError err)
{
    SpeedModel model(0, 0);
    try {
        ErrHelper::throwHttpError(err);
    } catch (BaseError &e) {
        qDebug()<<Q_FUNC_INFO<<"caught error: "<<e.getMessage();
        return BaseResponse<SpeedModel>(e.getCode(), e.getMessage(), model);
    }  catch (...) {
        qDebug()<<Q_FUNC_INFO<<"caught unkbnown error";
        ErrUnknown status;
        return BaseResponse<SpeedModel>(status.getCode(), status.getMessage(), model);
    }

    NoError status;
    return BaseResponse<SpeedModel>(status.getCode(), status.getMessage(), model);

}

