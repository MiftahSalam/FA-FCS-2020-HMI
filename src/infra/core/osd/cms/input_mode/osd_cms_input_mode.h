#ifndef OSDINPUTMODE_H
#define OSDINPUTMODE_H

#include <QTimer>

#include "src/infra/http/http_client_wrapper.h"
#include "src/infra/core/osd/cms/input_mode/osd_input_mode_request.h"
#include "src/infra/core/osd/model/input_mode/input_mode_model.h"
#include "src/shared/config/osd_cms_config.h"
#include "src/infra/http/i_cms_http.h"

class OSDCMSInputMode: public HttpClientWrapper, public ICmsHttp<InputModeModel, OSDInputModeRequest>
{
    Q_OBJECT
public:
    OSDCMSInputMode(OSDCMSInputMode &other) = delete;
    void operator=(const OSDCMSInputMode&) = delete;
    static OSDCMSInputMode* getInstance(
        HttpClientWrapper *httpClient,
        OSDCmsConfig *cmsConfig
        );

    void setDataMode(const bool confirm, const QString &dataFisis, const OSDInputModeRequest modeReq);

signals:
    void signal_setModeResponse(const QString datafisis, BaseResponse<InputModeModel> response, bool needConfirm);

protected:
    OSDCMSInputMode(
        HttpClientWrapper *parent = nullptr,
        OSDCmsConfig *cmsConfig = nullptr
        );

private slots:
    void onReplyFinished() override;

private:
    static OSDCMSInputMode *inputMode;
    OSDCmsConfig *cfgCms;

    void set(OSDInputModeRequest request) override;
    BaseResponse<InputModeModel> toResponse(QNetworkReply::NetworkError err, QByteArray raw) override;
};

#endif // OSDINPUTMODE_H
