#ifndef OSD_CMS_WEATHER_DATA_H
#define OSD_CMS_WEATHER_DATA_H

#include "src/domain/osd/repository/osd_weather_repository.h"
#include "src/infra/http/http_client_wrapper.h"
#include "src/infra/core/osd/cms/weather/osd_set_weather_request.h"
#include "src/infra/core/base_response.h"
#include "src/infra/core/osd/model/weather/weather_model.h"
#include "src/shared/config/osd_cms_config.h"
#include "src/infra/http/i_cms_http.h"

class OSDCMSWeatherData : public HttpClientWrapper, public ICmsHttp<WeatherModel, OSDSetWeatherRequest>
{
    Q_OBJECT
public:
    OSDCMSWeatherData(OSDCMSWeatherData &other) = delete;
    void operator=(const OSDCMSWeatherData&) = delete;
    static OSDCMSWeatherData* getInstance(
        HttpClientWrapper *httpClient,
        OSDCmsConfig *cmsConfig,
        OSDWeatherRepository* repoWeather = nullptr
        );

    void set(OSDSetWeatherRequest request) override;

signals:
    void signal_setWeatherResponse(BaseResponse<WeatherModel> response);

protected:
    OSDCMSWeatherData(
        HttpClientWrapper *parent = nullptr,
        OSDCmsConfig *cmsConfig = nullptr,
        OSDWeatherRepository* repoWeather = nullptr
        );

private slots:
    void onReplyFinished() override;

private:
    static OSDCMSWeatherData *weatherData;
    OSDCmsConfig *cfgCms;
    OSDWeatherRepository* repoWeather;

    BaseResponse<WeatherModel> toResponse(QByteArray raw) override;
    BaseResponse<WeatherModel> errorResponse(QNetworkReply::NetworkError err) override;
};
#endif // OSD_CMS_WEATHER_DATA_H
