#include "osd_cms_position_data.h"
#include "src/shared/common/errors/helper_err.h"
#include "src/shared/common/errors/err_json_parse.h"
#include "src/shared/common/errors/err_object_creation.h"
#include "src/shared/utils/utils.h"

OSDCMSPositionData* OSDCMSPositionData::positionData = nullptr;

OSDCMSPositionData::OSDCMSPositionData(
        HttpClientWrapper *parent,
        OSDCmsConfig *cmsConfig,
        OSDPositionRepository *repoPos
        ): HttpClientWrapper(parent), cfgCms(cmsConfig), repoPos(repoPos)
{
    if(parent == nullptr) {
        throw ErrObjectCreation();
    }
}

OSDCMSPositionData *OSDCMSPositionData::getInstance(
        HttpClientWrapper *httpClient = nullptr,
        OSDCmsConfig *cmsConfig = nullptr,
        OSDPositionRepository *repoPos
        )
{
    if (positionData == nullptr) {
        if(cmsConfig == nullptr) {
            throw ErrObjectCreation();
        }

        if(httpClient == nullptr) {
            throw ErrObjectCreation();
        }

        if(repoPos == nullptr) {
            throw ErrObjectCreation();
        }

        positionData = new OSDCMSPositionData(httpClient, cmsConfig, repoPos);
    }

    return positionData;
}

void OSDCMSPositionData::set(OSDSetPositionRequest request)
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

    BaseResponse<PositionModel> resp = errorResponse(httpResponse->error());
    if(resp.getHttpCode() != 0) {
        emit signal_setPositionResponse(resp);
        return;
    }

    resp = toResponse(respRaw);

    repoPos->SetPosition(OSDPositionEntity(
                             resp.getData().getLatitude(),
                             resp.getData().getLongitude(),
                             "manual",
                             "",
                             OSD_MODE::MANUAL //temp
                             ));

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
        BaseResponse<PositionModel> resp(respCode, respMsg, model);

        qDebug()<<Q_FUNC_INFO<<"resp"<<resp.getHttpCode()<<resp.getMessage()<<resp.getData().getLatitude()<<resp.getData().getLongitude();

        return resp;
    } catch (ErrJsonParse &e) {
        qDebug()<<Q_FUNC_INFO<<"caught error: "<<e.getMessage();
    }  catch (...) {
        qDebug()<<Q_FUNC_INFO<<"caught unkbnown error";
    }

    ErrUnknown status;
    PositionModel model(-91, -181);
    return BaseResponse<PositionModel>(status.getCode(), status.getMessage(), model);
}

BaseResponse<PositionModel> OSDCMSPositionData::errorResponse(QNetworkReply::NetworkError err)
{
    PositionModel model(-91, -181);
    try {
        ErrHelper::throwHttpError(err);
    } catch (BaseError &e) {
        qDebug()<<Q_FUNC_INFO<<"caught error: "<<e.getMessage();
        return BaseResponse<PositionModel>(e.getCode(), e.getMessage(), model);
    }  catch (...) {
        qDebug()<<Q_FUNC_INFO<<"caught unkbnown error";
        ErrUnknown status;
        return BaseResponse<PositionModel>(status.getCode(), status.getMessage(), model);
    }

    NoError status;
    return BaseResponse<PositionModel>(status.getCode(), status.getMessage(), model);
}
