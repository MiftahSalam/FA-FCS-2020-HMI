#include "weather_model.h"

WeatherModel::WeatherModel(double temp, double hum, double press):
    Temp(temp), Hum(hum), Press(press)
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

double WeatherModel::getTemp() const
{
    return Temp;
}

double WeatherModel::getHum() const
{
    return Hum;
}

double WeatherModel::getPress() const
{
    return Press;
}
