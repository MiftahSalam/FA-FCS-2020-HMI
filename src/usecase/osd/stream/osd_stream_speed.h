#ifndef OSDSTREAMSPEED_H
#define OSDSTREAMSPEED_H

#include <QObject>
#include <QWidget>

#include "src/domain/osd/repository/osd_speed_repository.h"
#include "src/infra/core/osd/cms/input_mode/osd_cms_input_mode.h"
#include "src/infra/messaging/tcp/tcp_messaging_wrapper.h"
#include "src/infra/core/osd/model/speed/speed_model.h"
#include "src/shared/config/messaging_tcp_config.h"
#include "src/usecase/osd/stream/IOSDStream.h"

class OSDStreamSpeed : public QObject, public IOSDStream<SpeedModel>
{
    Q_OBJECT
public:
    OSDStreamSpeed(OSDStreamSpeed &other) = delete;
    void operator=(const OSDStreamSpeed&) = delete;
    static OSDStreamSpeed* getInstance(
            TcpMessagingOpts *config,
            OSDSpeedRepository *repoSpeed,
            OSDCMSInputMode *modeService
            );

    BaseError check() override;

signals:
    void signalDataProcessed(SpeedModel data) override;

protected:
    OSDStreamSpeed(
            TcpMessagingOpts *config = nullptr,
            OSDSpeedRepository *repoSpeed = nullptr,
            OSDCMSInputMode *modeService = nullptr
            );

private slots:
    void onDataReceived(QByteArray data) override;

private:
    static OSDStreamSpeed *speedStream;

    TcpMessagingWrapper *consumer;
    TcpMessagingOpts *cfg;
    OSDSpeedRepository* _repoSpeed;
    OSDCMSInputMode *serviceMode;

    BaseError currentErr;

    void handleError(const QString &err) override;
};

#endif // OSDSTREAMSPEED_H
