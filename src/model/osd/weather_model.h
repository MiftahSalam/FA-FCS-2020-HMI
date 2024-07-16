#ifndef WEATHER_MODEL_H
#define WEATHER_MODEL_H

#include <QJsonObject>

class WeatherModel
{
public:
    WeatherModel(double temp, double hum, double press);
    WeatherModel(){};

    static WeatherModel FromJsonObject(QJsonObject obj);

    double getTemp() const;
    double getHum() const;
    double getPress() const;

private:
    double Temp, Hum, Press;
};

#endif // WEATHER_MODEL_H
