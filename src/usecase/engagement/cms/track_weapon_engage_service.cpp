#include "track_weapon_engage_service.h"
#include "src/shared/common/errors/err_http.h"
#include "src/shared/common/errors/helper_err.h"
#include "src/shared/common/errors/err_json_parse.h"
#include "src/shared/common/errors/err_object_creation.h"
#include "src/shared/utils/utils.h"

#ifdef USE_LOG4QT
#include <log4qt/logger.h>
LOG4QT_DECLARE_STATIC_LOGGER(logger, TrackWeaponEngageService)
#else
#include <QDebug>
#endif

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
    QNetworkRequest httpReq = QNetworkRequest(_cmsConfig->getInstance("")->getAssignUrl());
    httpReq.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply *curHttpResponse = nullptr;
    if (assign) {
        curHttpResponse = httpClient.post(httpReq, request.toJSON());
    } else {
        curHttpResponse = httpClient.sendCustomRequest(httpReq, "DELETE", request.toJSON());
    }
    connect(curHttpResponse, &QNetworkReply::finished, this, &TrackWeaponEngageService::onReplyFinished);
}

void TrackWeaponEngageService::sendAssignmentHB(TrackAssignRequest request)
{
    QNetworkRequest httpReq = QNetworkRequest(_cmsConfig->getInstance("")->getAssignHBUrl());
    httpReq.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    auto httpResponseHB = httpClient.post(httpReq, request.toJSON());

    connect(httpResponseHB, &QNetworkReply::finished, this, &TrackWeaponEngageService::onHBAssignReplyFinished);}

void TrackWeaponEngageService::sendResetAssignment(const QString weapon)
{
    QNetworkRequest httpReq = QNetworkRequest(_cmsConfig->getInstance("")->getResetAssignUrl()+"/"+weapon);

    auto httpResponse = httpClient.sendCustomRequest(httpReq, "DELETE", "{}");
    connect(httpResponse, &QNetworkReply::finished, this, &TrackWeaponEngageService::onResetAssignReplyFinished);

#ifdef USE_LOG4QT
    logger()->debug()<<Q_FUNC_INFO<<" -> url: "<<_cmsConfig->getInstance("")->getResetAssignUrl()+"/"+weapon;
#else
    qDebug()<<Q_FUNC_INFO<<"url: "<<_cmsConfig->getInstance("")->getResetAssignUrl()+"/"+weapon;
#endif
}

void TrackWeaponEngageService::onReplyFinished()
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

    BaseResponse<TrackAssignResponse> resp = errorHttpResponse(httpResponse->error());
    bool assign = isAssignResponse(httpResponse);
    if(resp.getHttpCode() != 0 || respRaw.isEmpty()) {
        emit signal_trackAssignmentResponse(resp, assign);
        return;
    }


    resp = toResponse(respRaw);

    emit signal_trackAssignmentResponse(resp, assign);

    httpResponse->deleteLater();
}

void TrackWeaponEngageService::onResetAssignReplyFinished()
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
}

void TrackWeaponEngageService::onHBAssignReplyFinished()
{
    QNetworkReply *objSender = dynamic_cast<QNetworkReply *>(sender());
    QByteArray respRaw = objSender->readAll();

#ifdef USE_LOG4QT
    logger()->debug()<<Q_FUNC_INFO<<" -> respRaw: "<<respRaw;
    logger()->debug()<<Q_FUNC_INFO<<" -> err: "<<objSender->error();
#else
    qDebug()<<Q_FUNC_INFO<<"respRaw: "<<respRaw;
    qDebug()<<Q_FUNC_INFO<<"err: "<<objSender->error();
#endif

    objSender->deleteLater();
}

BaseResponse<TrackAssignResponse> TrackWeaponEngageService::toResponse(QByteArray raw)
{
    TrackAssignResponse model;
    try {
        QJsonObject respObj = Utils::byteArrayToJsonObject(raw);
        int respCode = respObj["code"].toString().toInt();
        QString respMsg = respObj["message"].toString();
        QJsonObject respData = respObj["data"].toObject();
        //        TrackAssignResponse model(respData["id"].toInt(),"40mm");
        TrackAssignResponse model(respData["id"].toInt(),respData["weapon"].toString().toStdString());
        BaseResponse<TrackAssignResponse> resp(respCode, respMsg, model);

#ifdef USE_LOG4QT
        logger()->debug()<<Q_FUNC_INFO<<" -> resp. http code: "<<resp.getHttpCode()
                        <<", message: "<<resp.getMessage()
                       <<", track id: "<<resp.getData().getTrackId()
                      <<", weapon: "<<QString::fromStdString(resp.getData().getWeapon());
#else

        qDebug()<<Q_FUNC_INFO<<"resp"
               <<resp.getHttpCode()
              <<resp.getMessage()
             <<resp.getData().getTrackId()
            <<QString::fromStdString(resp.getData().getWeapon());
#endif

        ErrHelper::throwTrackEngageError(respCode);

        return resp;
    } catch (ErrJsonParse &e) {
#ifdef USE_LOG4QT
        logger()->error()<<Q_FUNC_INFO<<" -> caught error json parser: "<<e.getMessage();
#else
        qDebug()<<Q_FUNC_INFO<<"caught error json parser: "<<e.getMessage();
#endif
    }  catch (BaseError &e) {
#ifdef USE_LOG4QT
        logger()->error()<<Q_FUNC_INFO<<" -> caught error: "<<e.getMessage();
#else
        qWarning()<<Q_FUNC_INFO<<"caught error: "<<e.getMessage();
#endif
        return BaseResponse<TrackAssignResponse>(e.getCode(), e.getMessage(), model);
    }  catch (...) {
#ifdef USE_LOG4QT
        logger()->error()<<Q_FUNC_INFO<<" -> caught unkbnown error";
#else
        qWarning()<<Q_FUNC_INFO<<"caught unkbnown error";
#endif
    }

    ErrUnknown status;
    return BaseResponse<TrackAssignResponse>(status.getCode(), status.getMessage(), model);

}

BaseResponse<TrackAssignResponse> TrackWeaponEngageService::errorHttpResponse(QNetworkReply::NetworkError err)
{
    TrackAssignResponse model;
    try {
        switch (err) {
        case QNetworkReply::ConnectionRefusedError:
            throw ErrHttpConnRefused();
            break;
        default:
            break;
        }
    } catch (BaseError &e) {
#ifdef USE_LOG4QT
        logger()->error()<<Q_FUNC_INFO<<" -> caught error: "<<e.getMessage();
#else
        qWarning()<<Q_FUNC_INFO<<"caught error: "<<e.getMessage();
#endif
        return BaseResponse<TrackAssignResponse>(e.getCode(), e.getMessage(), model);
    }  catch (...) {
#ifdef USE_LOG4QT
        logger()->error()<<Q_FUNC_INFO<<" -> caught unkbnown error";
#else
        qWarning()<<Q_FUNC_INFO<<"caught unkbnown error";
#endif
    }

    NoError status;
    return BaseResponse<TrackAssignResponse>(status.getCode(), status.getMessage(), model);
}

bool TrackWeaponEngageService::isAssignResponse(QNetworkReply *httpResponse)
{
    auto headers = httpResponse->rawHeaderPairs();
    QString method;

#ifdef USE_LOG4QT
    foreach (auto header, headers) {
        logger()->debug()<<Q_FUNC_INFO<<" -> respHeaderRaw. key: "<<header.first<<", value: "<<header.second;
    }
#else
    qDebug()<<Q_FUNC_INFO<<"respHeaderRaw: "<<headers;
#endif

    foreach (auto header, headers) {
        if (header.first == "Method") {
            method = header.second;
            return method == "POST";
        }
    }
    return true;
}
