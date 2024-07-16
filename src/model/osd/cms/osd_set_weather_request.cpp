#include "osd_set_weather_request.h"
#include "qjsondocument.h"
#include "qjsonobject.h"

OSDSetWeatherRequest::OSDSetWeatherRequest(double _temp, double _hum, double _press):
    temp(_temp), hum(_hum), press(_press)
{

}

double OSDSetWeatherRequest::getTemp() const
{
    return temp;
}

double OSDSetWeatherRequest::getHum() const
{
    return hum;
}

double OSDSetWeatherRequest::getPress() const
{
    return press;
}

void OSDSetWeatherRequest::setTemp(const double _temp)
{
    temp = _temp;
}

void OSDSetWeatherRequest::setHum(const double _hum)
{
    hum = _hum;
}

void OSDSetWeatherRequest::setPress(const double _press)
{
    press = _press;
}

QByteArray OSDSetWeatherRequest::toJSON()
{
    QJsonObject obj;
    obj["temperature"] = temp;
    obj["humidity"] = hum;
    obj["pressure"] = press;

    return QJsonDocument(obj).toJson();

}
