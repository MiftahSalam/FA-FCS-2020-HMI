#ifndef OSDSTREAMSPEED_H
#define OSDSTREAMSPEED_H

#include "src/infra/messaging/tcp/tcp_messaging_wrapper.h"
#include "src/model/osd/speed_model.h"
#include "src/shared/config/messaging_tcp_config.h"
#include "src/usecase/osd/stream/IOSDStream.h"

#include <QObject>

class OSDStreamSpeed : public QObject, public IOSDStream<SpeedModel>
{
    Q_OBJECT
public:
    OSDStreamSpeed(OSDStreamSpeed &other) = delete;
    void operator=(const OSDStreamSpeed&) = delete;
    static OSDStreamSpeed* getInstance(TcpMessagingOpts *config);

    BaseError check() override;

signals:
    void signalDataProcessed(SpeedModel data) override;

protected:
    OSDStreamSpeed(TcpMessagingOpts *config = nullptr);

    // IOSDStream interface
private slots:
    void onDataReceived(QByteArray data) override;

private:
    static OSDStreamSpeed *speedStream;

    TcpMessagingWrapper *consumer;
    TcpMessagingOpts *cfg;
};

#endif // OSDSTREAMSPEED_H
