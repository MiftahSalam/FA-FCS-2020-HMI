#ifndef OSDSTREAMPOSITION_H
#define OSDSTREAMPOSITION_H

#include "src/infra/messaging/tcp/tcp_messaging_wrapper.h"
#include "src/model/osd/position_model.h"
//#include "src/shared/config/amqp_config.h"
#include "src/shared/config/messaging_tcp_config.h"
#include "src/usecase/osd/stream/IOSDStream.h"

#include <QObject>

class OSDStreamPosition : public QObject, public IOSDStream<PositionModel>
{
    Q_OBJECT
public:
    OSDStreamPosition(OSDStreamPosition &other) = delete;
    void operator=(const OSDStreamPosition&) = delete;
    static OSDStreamPosition* getInstance(TcpMessagingOpts *config);
//    static OSDStreamPosition* getInstance(AMQPConfig *config);

    void check();

signals:
    // IOSDStream interface
    void signalDataProcessed(PositionModel data) override;

protected:
    OSDStreamPosition(TcpMessagingOpts *config = nullptr);
//    OSDStreamPosition(AMQPConfig *config = nullptr);

    // IOSDStream interface
private slots:
    void onDataReceived(QByteArray data) override;

private:
    static OSDStreamPosition *positionStream;

//    AmqpConsumerWrapper *consumer;
    TcpMessagingWrapper *consumer;
    TcpMessagingOpts *cfg;
//    AMQPConfig *cfg;
};

#endif // OSDSTREAMPOSITION_H
