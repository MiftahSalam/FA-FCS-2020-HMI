#ifndef OSDSTREAM_H
#define OSDSTREAM_H

//#include "src/shared/config/amqp_config.h"
#include "src/infra/store/osd/osd_repository.h"
#include "src/shared/config/messaging_tcp_config.h"
#include "src/usecase/osd/cms/osd_cms.h"
#include "src/usecase/osd/stream/osd_stream_position.h"
#include "src/usecase/osd/stream/osd_stream_gyro.h"
#include "src/usecase/osd/stream/osd_stream_waterspeed.h"
#include "src/usecase/osd/stream/osd_stream_speed.h"
#include "src/usecase/osd/stream/osd_stream_wind.h"
#include "src/usecase/osd/stream/osd_stream_weather.h"
#include "src/usecase/osd/stream/stream_datetime.h"

#include <QObject>

class OSDStream : public QObject
{
    Q_OBJECT
public:
    OSDStream(
            QObject *parent = nullptr,
            MessagingTcpConfig *config = nullptr,
            OSDRepository *repoOSD = nullptr,
            OSDCMS *osdService= nullptr
            );
//    OSDStream(QObject *parent = nullptr, AMQPConfig *config = nullptr);

    OSDStreamPosition *getServiceOSDStreamPosition() const;
    OSDStreamGyro *getServiceOSDStreamGyro() const;
    OSDStreamWaterSpeed *getServiceOSDStreamWaterSpeed() const;
    OSDStreamSpeed *getServiceOSDStreamSpeed() const;
    OSDStreamWind *getServiceOSDStreamWind() const;
    OSDStreamWeather *getServiceOSDStreamWeather() const;
    StreamDateTime *getServiceStreamDateTime() const;
    
private:
    MessagingTcpConfig *consumerConfig;
//    AMQPConfig *cfgAmqp;
    OSDCMS *serviceOSDCMS;
    OSDStreamPosition *serviceOSDStreamPosition;
    OSDStreamGyro *serviceOSDStreamGyro;
    OSDStreamWaterSpeed *serviceOSDStreamWaterSpeed;
    OSDStreamSpeed *serviceOSDStreamSpeed;
    OSDStreamWind *serviceOSDStreamWind;
    OSDStreamWeather *serviceOSDStreamWeather;
    StreamDateTime *serviceStreamDateTime;
};

#endif // OSDSTREAM_H
