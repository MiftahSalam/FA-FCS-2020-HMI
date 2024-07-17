#include "osd_set_weather_request.h"
#include "qjsondocument.h"
#include "qjsonobject.h"

OSDSetWeatherRequest::OSDSetWeatherRequest(double _temp, double _hum, double _press):
    temperature(_temp), humidity(_hum), pressure(_press)
{

}

double OSDSetWeatherRequest::getTemperature() const
{
    return temperature;
}

double OSDSetWeatherRequest::getHumidity() const
{
    return humidity;
}

double OSDSetWeatherRequest::getPressure() const
{
    return pressure;
}

void OSDSetWeatherRequest::setTemperature(const double _temp)
{
    temperature = _temp;
}

void OSDSetWeatherRequest::setHumidity(const double _hum)
{
    humidity = _hum;
}

void OSDSetWeatherRequest::setPressure(const double _press)
{
    pressure = _press;
}

QByteArray OSDSetWeatherRequest::toJSON()
{
    QJsonObject obj;
    obj["temperature"] = temperature;
    obj["humidity"] = humidity;
    obj["pressure"] = pressure;

    return QJsonDocument(obj).toJson();

}
