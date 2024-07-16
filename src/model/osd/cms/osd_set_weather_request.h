#ifndef OSD_SET_WEATHER_REQUEST_H
#define OSD_SET_WEATHER_REQUEST_H

#include "src/model/base_request.h"

class OSDSetWeatherRequest: public BaseModel<OSDSetWeatherRequest>
{
public:
    OSDSetWeatherRequest();

    OSDSetWeatherRequest(double _temp, double _hum, double _press);

    double getTemp() const;
    double getHum() const;
    double getPress() const;

    void setTemp(const double _temp);
    void setHum(const double _hum);
    void setPress(const double _press);

private:
    double temp, hum, press;

public:
    QByteArray toJSON() override;
};

#endif // OSD_SET_WEATHER_REQUEST_H
