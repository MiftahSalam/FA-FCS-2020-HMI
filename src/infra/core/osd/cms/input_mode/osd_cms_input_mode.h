#ifndef OSDINPUTMODE_H
#define OSDINPUTMODE_H

#include <QTimer>

#include "src/domain/osd/repository/osd_mode_repository.h"
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
        OSDCmsConfig *cmsConfig,
        OSDInputModeRepository *modeRepo
        );

    void set(OSDInputModeRequest request) override;
    void setDataMode(const QString &dataFisis, const bool manualMode);
    const OSDInputModeEntity* getDataMode() const;

signals:
    void signal_setModeResponse(const QString datafisis, BaseResponse<InputModeModel> response, bool needConfirm);

protected:
    OSDCMSInputMode(
        HttpClientWrapper *parent = nullptr,
        OSDCmsConfig *cmsConfig = nullptr,
        OSDInputModeRepository *modeRepo = nullptr
        );

private slots:
    void onReplyFinished() override;

    void onTimerTimeout();
    void sync();

private:
    static OSDCMSInputMode *inputMode;
    OSDInputModeRepository *repoMode;
    OSDInputModeEntity *previousMode;
    OSDCmsConfig *cfgCms;

    QTimer *timer;

    bool synced;

    BaseResponse<InputModeModel> toResponse(QByteArray raw) override;
    BaseResponse<InputModeModel> errorResponse(QNetworkReply::NetworkError err) override;

    void resetToPrevMode();
};

#endif // OSDINPUTMODE_H
