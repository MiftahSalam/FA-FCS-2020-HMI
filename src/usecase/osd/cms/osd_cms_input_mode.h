#ifndef OSDINPUTMODE_H
#define OSDINPUTMODE_H


#include "src/domain/osd/repository/osd_base_repository.h"
#include "src/infra/http/http_client_wrapper.h"
#include "src/model/osd/cms/osd_input_mode_request.h"
#include "src/model/osd/input_mode_model.h"
#include "src/shared/config/osd_cms_config.h"
#include "src/usecase/osd/cms/i_osd_cms.h"

class OSDCMSInputMode: public HttpClientWrapper, public IOSDCMS<InputModeModel, OSDInputModeRequest>
{
    Q_OBJECT
public:
    OSDCMSInputMode(OSDCMSInputMode &other) = delete;
    void operator=(const OSDCMSInputMode&) = delete;
    static OSDCMSInputMode* getInstance(
            HttpClientWrapper *httpClient,
            OSDCmsConfig *cmsConfig,
            OSDBaseRepository* repoPos
            );

    void set(OSDInputModeRequest request) override;
    void setDataMode(const QString &dataFisis, const bool manualMode);

signals:
    void signal_setModeResponse(BaseResponse<InputModeModel> response);

protected:
    OSDCMSInputMode(
            HttpClientWrapper *parent = nullptr,
            OSDCmsConfig *cmsConfig = nullptr,
            OSDBaseRepository* repoPos = nullptr
            );

private slots:
    void onReplyFinished() override;

private:
    static OSDCMSInputMode *inputMode;
    OSDInputModeRequest currentMode;
    OSDCmsConfig *cfgCms;
    OSDBaseRepository* repoPos;
    QString lastUpdateMode;

    BaseResponse<InputModeModel> toResponse(QByteArray raw) override;
    BaseResponse<InputModeModel> errorResponse(QNetworkReply::NetworkError err) override;

    void resetToPrevMode();
};

#endif // OSDINPUTMODE_H
