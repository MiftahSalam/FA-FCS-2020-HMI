#ifndef OSD_SET_WEATHER_REQUEST_H
#define OSD_SET_WEATHER_REQUEST_H

#include "src/infra/core/base_request.h"

class OSDSetWeatherRequest: public BaseModel
{
public:
    OSDSetWeatherRequest();

    OSDSetWeatherRequest(double _temp, double _press, double _hum);

    double getTemperature() const;
    double getHumidity() const;
    double getPressure() const;

    void setTemperature(const double _temp);
    void setHumidity(const double _hum);
    void setPressure(const double _press);

    // BaseModel interface
    QByteArray toJSON() override;

private:
    double temperature, humidity, pressure;
};

#endif // OSD_SET_WEATHER_REQUEST_H
