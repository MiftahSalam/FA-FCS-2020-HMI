#include "track_weapon_engage_service.h"
#include "src/shared/common/errors/helper_err.h"
#include "src/shared/common/errors/err_json_parse.h"
#include "src/shared/common/errors/err_object_creation.h"
#include "src/shared/utils/utils.h"

TrackWeaponEngageService* TrackWeaponEngageService::instance = nullptr;

TrackWeaponEngageService::TrackWeaponEngageService(
        HttpClientWrapper *parent,
        TrackWeaponAssignConfig *cmsConfig
        ): HttpClientWrapper{parent}, _cmsConfig(cmsConfig)
{
    if(parent == nullptr) {
        throw ErrObjectCreation();
    }
}

TrackWeaponEngageService *TrackWeaponEngageService::getInstance(
        HttpClientWrapper *httpClient = nullptr,
        TrackWeaponAssignConfig *cmsConfig
        )
{
    if (instance == nullptr) {
        if(cmsConfig == nullptr) {
            throw ErrObjectCreation();
        }

        if(httpClient == nullptr) {
            throw ErrObjectCreation();
        }

        instance = new TrackWeaponEngageService(httpClient, cmsConfig);
    }

    return instance;
}

void TrackWeaponEngageService::sendAssignment(TrackAssignRequest request, bool assign)
{
    QNetworkRequest httpReq = QNetworkRequest(_cmsConfig->getInstance("")->getAssignUrl()+"/"+QString::number(request.getTrackId()));
    httpReq.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    if (assign) {
        httpResponse = httpClient.post(httpReq, request.toJSON());
    } else {
        httpResponse = httpClient.sendCustomRequest(httpReq, "DELETE", request.toJSON());
    }
    connect(httpResponse, &QNetworkReply::finished, this, &TrackWeaponEngageService::onReplyFinished);

}

void TrackWeaponEngageService::onReplyFinished()
{
    QByteArray respRaw = httpResponse->readAll();

    qDebug()<<Q_FUNC_INFO<<"respHeaderRaw: "<<httpResponse->rawHeaderPairs();
    qDebug()<<Q_FUNC_INFO<<"respRaw: "<<respRaw;
    qDebug()<<Q_FUNC_INFO<<"err: "<<httpResponse->error();

    BaseResponse<TrackAssignResponse> resp = errorResponse(httpResponse->error());
    if(resp.getHttpCode() != 0 || respRaw.isEmpty()) {
        // TODO: check header to choose signal flag to emit
        emit signal_trackAssignmentResponse(resp, true);
        return;
    }

    resp = toResponse(respRaw);

    // TODO: check header to choose signal flag to emit
    emit signal_trackAssignmentResponse(resp, true);
}

BaseResponse<TrackAssignResponse> TrackWeaponEngageService::toResponse(QByteArray raw)
{
    try {
        QJsonObject respObj = Utils::byteArrayToJsonObject(raw);
        int respCode = respObj["code"].toInt();
        QString respMsg = respObj["message"].toString();
        QJsonObject respData = respObj["data"].toObject();
        TrackAssignResponse model(respData["id"].toInt(),"40mm"); //TODO: set weapon from response body
//        TrackAssignResponse model(respData["id"].toInt(),respData["weapon"].toString().toStdString());
        BaseResponse<TrackAssignResponse> resp(respCode, respMsg, model);

        qDebug()<<Q_FUNC_INFO<<"resp"
               <<resp.getHttpCode()
              <<resp.getMessage()
             <<resp.getData().getTrackId()
            <<QString::fromStdString(resp.getData().getWeapon());

        return resp;
    } catch (ErrJsonParse &e) {
        qDebug()<<Q_FUNC_INFO<<"caught error: "<<e.getMessage();
    }  catch (...) {
        qDebug()<<Q_FUNC_INFO<<"caught unkbnown error";
    }

    ErrUnknown status;
    TrackAssignResponse model;
    return BaseResponse<TrackAssignResponse>(status.getCode(), status.getMessage(), model);

}

BaseResponse<TrackAssignResponse> TrackWeaponEngageService::errorResponse(QNetworkReply::NetworkError err)
{
    TrackAssignResponse model;
    try {
        ErrHelper::throwHttpError(err);
    } catch (BaseError &e) {
        qDebug()<<Q_FUNC_INFO<<"caught error: "<<e.getMessage();
        return BaseResponse<TrackAssignResponse>(e.getCode(), e.getMessage(), model);
    }  catch (...) {
        qDebug()<<Q_FUNC_INFO<<"caught unkbnown error";
        ErrUnknown status;
        return BaseResponse<TrackAssignResponse>(status.getCode(), status.getMessage(), model);
    }

    NoError status;
    return BaseResponse<TrackAssignResponse>(status.getCode(), status.getMessage(), model);

}
