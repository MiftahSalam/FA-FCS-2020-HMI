#ifndef OSDSTREAMSPEED_H
#define OSDSTREAMSPEED_H

#include <QObject>
#include <QWidget>

#include "src/infra/core/osd/model/speed/speed_stream_model.h"
#include "src/infra/messaging/tcp/tcp_messaging_wrapper.h"
#include "src/infra/messaging/IOSDStream.h"
#include "src/shared/config/messaging_tcp_config.h"

class OSDStreamSpeed : public QObject, public IOSDStream<SpeedStreamModel>
{
    Q_OBJECT
public:
    OSDStreamSpeed(OSDStreamSpeed &other) = delete;
    void operator=(const OSDStreamSpeed&) = delete;
    static OSDStreamSpeed* getInstance(
            TcpMessagingOpts *config
            );

    BaseError check() override;

signals:
    void signalDataProcessed(SpeedStreamModel data) override;

protected:
    OSDStreamSpeed(
        TcpMessagingOpts *config = nullptr
        );

private slots:
    void onDataReceived(QByteArray data) override;

private:
    static OSDStreamSpeed *speedStream;

    TcpMessagingWrapper *consumer;
    TcpMessagingOpts *cfg;

    BaseError currentErr;

    void handleError(const QString &err) override;
};

#endif // OSDSTREAMSPEED_H
