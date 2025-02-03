#ifndef WEATHER_STREAM_MODEL_H
#define WEATHER_STREAM_MODEL_H

#include "qjsonobject.h"

#include "src/infra/core/osd/model/osd_base_model.h"

class WeatherStreamModel: public OSDBaseModel
{
public:
    WeatherStreamModel();
    WeatherStreamModel(OSDBaseModel base_data, double temperature, double pressure, double humidity);

    static WeatherStreamModel fromJsonObject(QJsonObject obj);

    double getTemperature() const;
    double getPressure() const;
    double getHumidity() const;

private:
    double _temperature;
    double _pressure;
    double _humidity;
};

#endif // WEATHER_STREAM_MODEL_H
