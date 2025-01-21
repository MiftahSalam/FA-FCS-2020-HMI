#ifndef OSDSTREAMWIND_H
#define OSDSTREAMWIND_H

#include "src/domain/osd/repository/osd_wind_repository.h"
#include "src/infra/core/cms/osd/input_mode/osd_cms_input_mode.h"
#include "src/infra/messaging/tcp/tcp_messaging_wrapper.h"
#include "src/model/osd/wind_model.h"
#include "src/shared/config/messaging_tcp_config.h"
#include "src/usecase/osd/stream/IOSDStream.h"

#include <QObject>

class OSDStreamWind : public QObject, public IOSDStream<WindModel>
{
    Q_OBJECT
public:
    OSDStreamWind(OSDStreamWind &other) = delete;
    void operator=(const OSDStreamWind&) = delete;
    static OSDStreamWind* getInstance(
            TcpMessagingOpts *config,
            OSDWindRepository *_repoWP,
            OSDCMSInputMode *modeService
            );

    BaseError check() override;

signals:
    // IOSDStream interface
    void signalDataProcessed(WindModel data) override;

protected:
    OSDStreamWind(
            TcpMessagingOpts *config = nullptr,
            OSDWindRepository *repoWind = nullptr,
            OSDCMSInputMode *modeService = nullptr
            );

    // IOSDStream interface
private slots:
    void onDataReceived(QByteArray data) override;

private:
    static OSDStreamWind *windStream;

    TcpMessagingWrapper *consumer;
    TcpMessagingOpts *cfg;
    OSDWindRepository* _repoWind;
    OSDCMSInputMode *serviceMode;
    // TODO: add input mode repo

    BaseError currentErr;

    void handleError(const QString &err) override;
};

#endif // OSDSTREAMWIND_H
