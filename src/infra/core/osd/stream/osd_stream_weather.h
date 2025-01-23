#ifndef OSD_STREAM_WEATHER_H
#define OSD_STREAM_WEATHER_H

#include <QObject>
#include <QWidget>

#include "src/domain/osd/repository/osd_weather_repository.h"
#include "src/infra/core/osd/cms/input_mode/osd_cms_input_mode.h"
#include "src/infra/core/osd/model/weather/weather_model.h"
#include "src/infra/messaging/tcp/tcp_messaging_wrapper.h"
#include "src/infra/messaging/IOSDStream.h"
#include "src/shared/config/messaging_tcp_config.h"

class OSDStreamWeather : public QObject, public IOSDStream<WeatherModel>
{
    Q_OBJECT
public:
    OSDStreamWeather(OSDStreamWeather &other) = delete;
    void operator=(const OSDStreamWeather&) = delete;
    static OSDStreamWeather* getInstance(
        TcpMessagingOpts *config,
        OSDWeatherRepository *repoWeather,
        OSDCMSInputMode *modeService
        );

    BaseError check() override;

signals:
    void signalDataProcessed(WeatherModel data) override;

protected:
    OSDStreamWeather(
        TcpMessagingOpts *config = nullptr,
        OSDWeatherRepository *repoWeather = nullptr,
        OSDCMSInputMode *modeService = nullptr
        );

private slots:
    void onDataReceived(QByteArray data) override;

private:
    static OSDStreamWeather *weatherStream;

    TcpMessagingWrapper *consumer;
    TcpMessagingOpts *cfg;
    OSDWeatherRepository* _repoWeather;
    OSDCMSInputMode *serviceMode;
    BaseError currentErr;

    void handleError(const QString &err) override;
};

#endif // OSD_STREAM_WEATHER_H
