#include "weather_cms_response_model.h"

WeatherResponseModel::WeatherResponseModel(double temp, double press, double hum):
    Temperature(temp), Pressure(press), Humidity(hum)
{

}

WeatherResponseModel WeatherResponseModel::FromJsonObject(QJsonObject obj)
{
    WeatherResponseModel model(
        obj.value("temperature").toDouble(),
        obj.value("pressure").toDouble(),
        obj.value("humidity").toDouble()
        );

    return model;
}

double WeatherResponseModel::getTemperature() const
{
    return Temperature;
}

double WeatherResponseModel::getHumidity() const
{
    return Humidity;
}

double WeatherResponseModel::getPressure() const
{
    return Pressure;
}
