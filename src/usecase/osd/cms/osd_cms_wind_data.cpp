#include "osd_cms_wind_data.h"
#include "src/shared/common/errors/helper_err.h"
#include "src/shared/common/errors/err_json_parse.h"
#include "src/shared/common/errors/err_object_creation.h"
#include "src/shared/utils/utils.h"

OSDCMSWindData* OSDCMSWindData::windData = nullptr;

OSDCMSWindData::OSDCMSWindData(
        HttpClientWrapper *parent,
        OSDCmsConfig *cmsConfig,
        OSDWindRepository *repoWind
        ): HttpClientWrapper(parent), cfgCms(cmsConfig), repoWind(repoWind)
{
    if(parent == nullptr) {
        throw ErrObjectCreation();
    }
}

OSDCMSWindData *OSDCMSWindData::getInstance(
        HttpClientWrapper *httpClient = nullptr,
        OSDCmsConfig *cmsConfig = nullptr,
        OSDWindRepository *repoWind
        )
{
    if (windData == nullptr) {
        if(cmsConfig == nullptr) {
            throw ErrObjectCreation();
        }

        if(httpClient == nullptr) {
            throw ErrObjectCreation();
        }

        if(repoWind == nullptr) {
            throw ErrObjectCreation();
        }
        windData = new OSDCMSWindData(httpClient, cmsConfig, repoWind);
    }
    return windData;
}

void OSDCMSWindData::set(OSDSetWindRequest request)
{
    QNetworkRequest httpReq = QNetworkRequest(cfgCms->getInstance("")->getManualDataUrl()+"/wind");
    httpReq.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    httpResponse = httpClient.put(httpReq, request.toJSON());
    connect(httpResponse, &QNetworkReply::finished, this, &OSDCMSWindData::onReplyFinished);
}

void OSDCMSWindData::onReplyFinished()
{
    QByteArray respRaw = httpResponse->readAll();

    qDebug()<<Q_FUNC_INFO<<"respRaw: "<<respRaw;
    qDebug()<<Q_FUNC_INFO<<"err: "<<httpResponse->error();

    BaseResponse<WindModel> resp = errorResponse(httpResponse->error());
    if(resp.getHttpCode() != 0) {
        emit signal_setWindResponse(resp);
        return;
    }

    resp = toResponse(respRaw);

    repoWind->SetWind(OSDWindEntity(
                             resp.getData().getSpeed(),
                             resp.getData().getDirection(),
                             "manual",
                             "",
                             OSD_MODE::MANUAL
                             ));

    emit signal_setWindResponse(resp);
}

BaseResponse<WindModel> OSDCMSWindData::toResponse(QByteArray raw)
{
    try {
        QJsonObject respObj = Utils::byteArrayToJsonObject(raw);
        int respCode = respObj["code"].toInt();
        QString respMsg = respObj["message"].toString();
        QJsonObject respData = respObj["data"].toObject();
        WindModel model(respData["speed"].toDouble(),respData["direction"].toDouble());
        BaseResponse<WindModel> resp(respCode, respMsg, model);

        qDebug()<<Q_FUNC_INFO<<"resp"<<resp.getHttpCode()<<resp.getMessage()<<resp.getData().getSpeed()<<resp.getData().getDirection();

        return resp;

    } catch (ErrJsonParse &e) {
        qDebug()<<Q_FUNC_INFO<<"caught error: "<<e.getMessage();
    }  catch (...) {
        qDebug()<<Q_FUNC_INFO<<"caught unkbnown error";
    }

    ErrUnknown status;
    WindModel model(0, 0);
    return BaseResponse<WindModel>(status.getCode(), status.getMessage(), model);
}

BaseResponse<WindModel> OSDCMSWindData::errorResponse(QNetworkReply::NetworkError err)
{
    WindModel model(0, 0);
    try {
        ErrHelper::throwHttpError(err);
    } catch (BaseError &e) {
        qDebug()<<Q_FUNC_INFO<<"caught error: "<<e.getMessage();
        return BaseResponse<WindModel>(e.getCode(), e.getMessage(), model);
    }  catch (...) {
        qDebug()<<Q_FUNC_INFO<<"caught unkbnown error";
        ErrUnknown status;
        return BaseResponse<WindModel>(status.getCode(), status.getMessage(), model);
    }

    NoError status;
    return BaseResponse<WindModel>(status.getCode(), status.getMessage(), model);
}
