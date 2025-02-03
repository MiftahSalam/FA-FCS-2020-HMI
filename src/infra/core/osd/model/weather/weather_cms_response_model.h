#ifndef WEATHER_CMS_RESPONSE_MODEL_H
#define WEATHER_CMS_RESPONSE_MODEL_H

#include "src/infra/core/osd/model/osd_base_model.h"
#include <QJsonObject>

class WeatherResponseModel: public OSDBaseModel
{
public:
    WeatherResponseModel(double temp, double press, double hum);
    WeatherResponseModel(){};

    static WeatherResponseModel FromJsonObject(QJsonObject obj);

    double getTemperature() const;
    double getHumidity() const;
    double getPressure() const;

private:
    double Temperature, Humidity, Pressure;
};

#endif // WEATHER_CMS_RESPONSE_MODEL_H
