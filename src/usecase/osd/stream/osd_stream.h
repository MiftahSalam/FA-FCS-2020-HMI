#ifndef OSDSTREAM_H
#define OSDSTREAM_H

//#include "src/shared/config/amqp_config.h"
#include "src/shared/config/messaging_tcp_config.h"
#include "src/usecase/osd/stream/osd_stream_position.h"
#include "src/usecase/osd/stream/osd_stream_speed.h"

#include <QObject>

class OSDStream : public QObject
{
    Q_OBJECT
public:
    OSDStream(QObject *parent = nullptr, MessagingTcpConfig *config = nullptr);
//    OSDStream(QObject *parent = nullptr, AMQPConfig *config = nullptr);

    OSDStreamPosition *getServiceOSDStreamPosition() const;
    OSDStreamSpeed *getServiceOSDStreamSpeed() const;

private:
    MessagingTcpConfig *consumerConfig;
//    AMQPConfig *cfgAmqp;
    OSDStreamPosition *serviceOSDStreamPosition;
    OSDStreamSpeed *serviceOSDStreamSpeed;
};

#endif // OSDSTREAM_H
