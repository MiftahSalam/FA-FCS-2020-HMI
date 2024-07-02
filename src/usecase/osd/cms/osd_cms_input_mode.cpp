#include "osd_cms_input_mode.h"
#include "src/shared/common/errors/helper_err.h"
#include "src/shared/common/errors/err_json_parse.h"
#include "src/shared/common/errors/err_object_creation.h"
#include "src/shared/utils/utils.h"

OSDCMSInputMode* OSDCMSInputMode::inputMode = nullptr;

OSDCMSInputMode::OSDCMSInputMode(
        HttpClientWrapper *parent,
        OSDCmsConfig *cmsConfig,
        OSDBaseRepository *repoPos
        ): HttpClientWrapper(parent), cfgCms(cmsConfig), repoPos(repoPos)
{
    if(parent == nullptr) {
        throw ErrObjectCreation();
    }
}

OSDCMSInputMode *OSDCMSInputMode::getInstance(
        HttpClientWrapper *httpClient = nullptr,
        OSDCmsConfig *cmsConfig = nullptr,
        OSDBaseRepository *repoPos =nullptr
        )
{
    if (inputMode == nullptr) {
        if(cmsConfig == nullptr) {
            throw ErrObjectCreation();
        }

        if(httpClient == nullptr) {
            throw ErrObjectCreation();
        }

        if(repoPos == nullptr) {
            throw ErrObjectCreation();
        }

        inputMode = new OSDCMSInputMode(httpClient, cmsConfig);
    }

    return inputMode;
}

void OSDCMSInputMode::set(OSDInputModeRequest request)
{
    QNetworkRequest httpReq = QNetworkRequest(cfgCms->getInstance("")->getModeUrl());
    httpReq.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    httpResponse = httpClient.put(httpReq, request.toJSON());
    connect(httpResponse, &QNetworkReply::finished, this, &OSDCMSInputMode::onReplyFinished);
}

void OSDCMSInputMode::onReplyFinished()
{
    QByteArray respRaw = httpResponse->readAll();

    qDebug()<<Q_FUNC_INFO<<"respRaw: "<<respRaw;
    qDebug()<<Q_FUNC_INFO<<"err: "<<httpResponse->error();

    BaseResponse<InputModeModel> resp = errorResponse(httpResponse->error());
    if(resp.getHttpCode() != 0) {
        emit signal_setModeResponse(resp);
        return;
    }

    resp = toResponse(respRaw);

    //TODO: update repo
//    repoPos->SetEntity(); //temp

    emit signal_setModeResponse(resp);
}

BaseResponse<InputModeModel> OSDCMSInputMode::toResponse(QByteArray raw)
{
    try {
        QJsonObject respObj = Utils::byteArrayToJsonObject(raw);
        int respCode = respObj["code"].toInt();
        QString respMsg = respObj["message"].toString();
        QJsonObject respData = respObj["data"].toObject();
        InputModeModel model(
                    respData["position"].toBool(),
                respData["speed"].toBool(),
                respData["inersia"].toBool()
                );
        BaseResponse<InputModeModel> resp(respCode, respMsg, &model);

        return resp;
    } catch (ErrJsonParse &e) {
        qDebug()<<Q_FUNC_INFO<<"caught error: "<<e.getMessage();
    }  catch (...) {
        qDebug()<<Q_FUNC_INFO<<"caught unkbnown error";
    }

    ErrUnknown status;
    return BaseResponse<InputModeModel>(status.getCode(), status.getMessage(), nullptr);
}

BaseResponse<InputModeModel> OSDCMSInputMode::errorResponse(QNetworkReply::NetworkError err)
{
    try {
        ErrHelper::throwHttpError(err);
    } catch (BaseError &e) {
        qDebug()<<Q_FUNC_INFO<<"caught error: "<<e.getMessage();
        return BaseResponse<InputModeModel>(e.getCode(), e.getMessage(), nullptr);
    }  catch (...) {
        qDebug()<<Q_FUNC_INFO<<"caught unkbnown error";
        ErrUnknown status;
        return BaseResponse<InputModeModel>(status.getCode(), status.getMessage(), nullptr);
    }

    NoError status;
    return BaseResponse<InputModeModel>(status.getCode(), status.getMessage(), nullptr);
}
