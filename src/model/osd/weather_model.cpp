#include "weather_model.h"

WeatherModel::WeatherModel(double temp, double hum, double press):
    Temperature(temp), Humidity(hum), Pressure(press)
{

}

WeatherModel WeatherModel::FromJsonObject(QJsonObject obj)
{
    WeatherModel model(
        obj.value("temperature").toDouble(),
        obj.value("humidity").toDouble(),
        obj.value("pressure").toDouble()
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
