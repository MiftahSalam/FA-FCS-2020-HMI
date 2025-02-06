#ifndef OSDSTREAM_H
#define OSDSTREAM_H

//#include "src/shared/config/amqp_config.h"
#include "src/shared/config/messaging_tcp_config.h"
#include "src/infra/core/osd/stream/osd_stream_position.h"
#include "src/infra/core/osd/stream/osd_stream_gyro.h"
#include "src/infra/core/osd/stream/osd_stream_waterspeed.h"
#include "src/infra/core/osd/stream/osd_stream_speed.h"
#include "src/infra/core/osd/stream/osd_stream_wind.h"
#include "src/infra/core/osd/stream/osd_stream_weather.h"
#include "src/infra/core/osd/stream/osd_stream_datetime.h"

#include <QObject>

class OSDStream : public QObject
{
    Q_OBJECT
public:
    OSDStream(
            QObject *parent = nullptr,
            MessagingTcpConfig *config = nullptr
            );
//    OSDStream(QObject *parent = nullptr, AMQPConfig *config = nullptr);

    OSDStreamPosition *getServiceOSDStreamPosition() const;
    OSDStreamGyro *getServiceOSDStreamGyro() const;
    OSDStreamWaterSpeed *getServiceOSDStreamWaterSpeed() const;
    OSDStreamSpeed *getServiceOSDStreamSpeed() const;
    OSDStreamWind *getServiceOSDStreamWind() const;
    OSDStreamWeather *getServiceOSDStreamWeather() const;
    OSDStreamDateTime *getServiceStreamDateTime() const;
    
private:
    MessagingTcpConfig *consumerConfig;
//    AMQPConfig *cfgAmqp;
    OSDStreamPosition *serviceOSDStreamPosition;
    OSDStreamGyro *serviceOSDStreamGyro;
    OSDStreamWaterSpeed *serviceOSDStreamWaterSpeed;
    OSDStreamSpeed *serviceOSDStreamSpeed;
    OSDStreamWind *serviceOSDStreamWind;
    OSDStreamWeather *serviceOSDStreamWeather;
    OSDStreamDateTime *serviceStreamDateTime;
};

#endif // OSDSTREAM_H
