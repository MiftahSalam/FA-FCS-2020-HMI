#ifndef OSD_CMS_WEATHER_DATA_H
#define OSD_CMS_WEATHER_DATA_H

#include "src/infra/http/http_client_wrapper.h"
#include "src/infra/core/osd/cms/weather/osd_set_weather_request.h"
#include "src/infra/core/base_response.h"
#include "src/infra/core/osd/model/weather/weather_cms_response_model.h"
#include "src/shared/config/osd_cms_config.h"
#include "src/infra/http/i_cms_http.h"

class OSDCMSWeatherData : public HttpClientWrapper, public ICmsHttp<WeatherResponseModel, OSDSetWeatherRequest>
{
    Q_OBJECT
public:
    OSDCMSWeatherData(OSDCMSWeatherData &other) = delete;
    void operator=(const OSDCMSWeatherData&) = delete;
    static OSDCMSWeatherData* getInstance(
        HttpClientWrapper *httpClient,
        OSDCmsConfig *cmsConfig);

    void set(OSDSetWeatherRequest request) override;

signals:
    void signal_setWeatherResponse(BaseResponse<WeatherResponseModel> response);

protected:
    OSDCMSWeatherData(
        HttpClientWrapper *parent = nullptr,
        OSDCmsConfig *cmsConfig = nullptr
        );

private slots:
    void onReplyFinished() override;

private:
    static OSDCMSWeatherData *weatherData;
    OSDCmsConfig *cfgCms;

    BaseResponse<WeatherResponseModel> toResponse(QNetworkReply::NetworkError err, QByteArray raw) override;
};
#endif // OSD_CMS_WEATHER_DATA_H
