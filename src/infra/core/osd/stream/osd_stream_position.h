#ifndef OSDSTREAMPOSITION_H
#define OSDSTREAMPOSITION_H

#include "src/infra/core/osd/model/position/position_stream_model.h"
#include "src/infra/messaging/tcp/tcp_messaging_wrapper.h"
#include "src/infra/messaging/IOSDStream.h"
//#include "src/shared/config/amqp_config.h"
#include "src/shared/config/messaging_tcp_config.h"

#include <QObject>

class OSDStreamPosition : public QObject, public IOSDStream<PositionStreamModel>
{
    Q_OBJECT
public:
    OSDStreamPosition(OSDStreamPosition &other) = delete;
    void operator=(const OSDStreamPosition&) = delete;
    static OSDStreamPosition* getInstance(
            TcpMessagingOpts *config
            );
//    static OSDStreamPosition* getInstance(AMQPConfig *config);

    BaseError check() override;

signals:
    // IOSDStream interface
    void signalDataProcessed(PositionStreamModel data) override;

protected:
    OSDStreamPosition(
            TcpMessagingOpts *config = nullptr
            );

    // IOSDStream interface
private slots:
    void onDataReceived(QByteArray data) override;

private:
    static OSDStreamPosition *positionStream;

//    AmqpConsumerWrapper *consumer;
    TcpMessagingWrapper *consumer;
    TcpMessagingOpts *cfg;
//    AMQPConfig *cfg;

    BaseError currentErr;

    void handleError(const QString &err) override;
};

#endif // OSDSTREAMPOSITION_H
