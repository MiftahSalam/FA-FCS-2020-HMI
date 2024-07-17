#ifndef OSD_SET_WEATHER_REQUEST_H
#define OSD_SET_WEATHER_REQUEST_H

#include "src/model/base_request.h"

class OSDSetWeatherRequest: public BaseModel<OSDSetWeatherRequest>
{
public:
    OSDSetWeatherRequest();

    OSDSetWeatherRequest(double _temp, double _hum, double _press);

    double getTemperature() const;
    double getHumidity() const;
    double getPressure() const;

    void setTemperature(const double _temp);
    void setHumidity(const double _hum);
    void setPressure(const double _press);

private:
    double temperature, humidity, pressure;

public:
    QByteArray toJSON() override;
};

#endif // OSD_SET_WEATHER_REQUEST_H
