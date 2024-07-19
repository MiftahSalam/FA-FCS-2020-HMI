#include "weather_model.h"

WeatherModel::WeatherModel(double temp, double press, double hum):
    Temperature(temp), Pressure(press), Humidity(hum)
{

}

WeatherModel WeatherModel::FromJsonObject(QJsonObject obj)
{
    WeatherModel model(
        obj.value("temperature").toDouble(),
        obj.value("pressure").toDouble(),
        obj.value("humidity").toDouble()
        );

    return model;
}

double WeatherModel::getTemperature() const
{
    return Temperature;
}

double WeatherModel::getHumidity() const
{
    return Humidity;
}

double WeatherModel::getPressure() const
{
    return Pressure;
}
