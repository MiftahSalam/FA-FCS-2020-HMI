#ifndef OSDSTREAMWATERSPEED_H
#define OSDSTREAMWATERSPEED_H

#include "src/domain/osd/repository/osd_waterspeed_repository.h"
#include "src/infra/messaging/tcp/tcp_messaging_wrapper.h"
#include "src/model/osd/waterspeed_model.h"
#include "src/shared/config/messaging_tcp_config.h"
#include "src/usecase/osd/cms/osd_cms_input_mode.h"
#include "src/usecase/osd/stream/IOSDStream.h"

#include <QObject>

class OSDStreamWaterSpeed : public QObject, public IOSDStream<WaterSpeedModel>
{
    Q_OBJECT
public:
    OSDStreamWaterSpeed(OSDStreamWaterSpeed &other) = delete;
    void operator=(const OSDStreamWaterSpeed&) = delete;
    static OSDStreamWaterSpeed* getInstance(
            TcpMessagingOpts *config,
            OSDWaterSpeedRepository *_repoWP,
            OSDCMSInputMode *modeService
            );

    BaseError check() override;

signals:
    // IOSDStream interface
    void signalDataProcessed(WaterSpeedModel data) override;

protected:
    OSDStreamWaterSpeed(
            TcpMessagingOpts *config = nullptr,
            OSDWaterSpeedRepository *repoWP = nullptr,
            OSDCMSInputMode *modeService = nullptr
            );

    // IOSDStream interface
private slots:
    void onDataReceived(QByteArray data) override;

private:
    static OSDStreamWaterSpeed *waterspeedStream;

    TcpMessagingWrapper *consumer;
    TcpMessagingOpts *cfg;
    OSDWaterSpeedRepository* _repoWP;
    OSDCMSInputMode *serviceMode;
    // TODO: add input mode repo

    BaseError currentErr;

    void handleError(const QString &err) override;
};

#endif // OSDSTREAMWATERSPEED_H
