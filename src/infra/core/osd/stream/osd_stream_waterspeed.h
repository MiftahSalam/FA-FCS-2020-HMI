#ifndef OSDSTREAMWATERSPEED_H
#define OSDSTREAMWATERSPEED_H

#include "src/infra/core/osd/model/water_speed/water_speed_stream_model.h"
#include "src/infra/messaging/tcp/tcp_messaging_wrapper.h"
#include "src/infra/messaging/IOSDStream.h"
#include "src/shared/config/messaging_tcp_config.h"

#include <QObject>

class OSDStreamWaterSpeed : public QObject, public IOSDStream<WaterSpeedStreamModel>
{
    Q_OBJECT
public:
    OSDStreamWaterSpeed(OSDStreamWaterSpeed &other) = delete;
    void operator=(const OSDStreamWaterSpeed&) = delete;
    static OSDStreamWaterSpeed* getInstance(
            TcpMessagingOpts *config
            );

    BaseError check() override;

signals:
    // IOSDStream interface
    void signalDataProcessed(WaterSpeedStreamModel data) override;

protected:
    OSDStreamWaterSpeed(
        TcpMessagingOpts *config = nullptr);

    // IOSDStream interface
private slots:
    void onDataReceived(QByteArray data) override;

private:
    static OSDStreamWaterSpeed *waterspeedStream;

    TcpMessagingWrapper *consumer;
    TcpMessagingOpts *cfg;

    BaseError currentErr;

    void handleError(const QString &err) override;
};

#endif // OSDSTREAMWATERSPEED_H
