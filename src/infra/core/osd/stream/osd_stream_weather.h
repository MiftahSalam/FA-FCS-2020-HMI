#ifndef OSD_STREAM_WEATHER_H
#define OSD_STREAM_WEATHER_H

#include <QObject>
#include <QWidget>

#include "src/infra/core/osd/model/weather/weather_stream_model.h"
#include "src/infra/messaging/tcp/tcp_messaging_wrapper.h"
#include "src/infra/messaging/IOSDStream.h"
#include "src/shared/config/messaging_tcp_config.h"

class OSDStreamWeather : public QObject, public IOSDStream<WeatherStreamModel>
{
    Q_OBJECT
public:
    OSDStreamWeather(OSDStreamWeather &other) = delete;
    void operator=(const OSDStreamWeather&) = delete;
    static OSDStreamWeather* getInstance(
        TcpMessagingOpts *config);

    BaseError check() override;

signals:
    void signalDataProcessed(WeatherStreamModel data) override;

protected:
    OSDStreamWeather(
        TcpMessagingOpts *config = nullptr
        );

private slots:
    void onDataReceived(QByteArray data) override;

private:
    static OSDStreamWeather *weatherStream;

    TcpMessagingWrapper *consumer;
    TcpMessagingOpts *cfg;

    BaseError currentErr;

    void handleError(const QString &err) override;
};

#endif // OSD_STREAM_WEATHER_H
