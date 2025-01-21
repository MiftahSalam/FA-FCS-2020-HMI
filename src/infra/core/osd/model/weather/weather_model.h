#ifndef WEATHER_MODEL_H
#define WEATHER_MODEL_H

#include <QJsonObject>

class WeatherModel
{
public:
    WeatherModel(double temp, double press, double hum);
    WeatherModel(){};

    static WeatherModel FromJsonObject(QJsonObject obj);

    double getTemperature() const;
    double getHumidity() const;
    double getPressure() const;

private:
    double Temperature, Humidity, Pressure;
};

#endif // WEATHER_MODEL_H
