#include "osd_input_mode_request.h"
#include "qjsondocument.h"
#include "qjsonobject.h"

OSDInputModeRequest::OSDInputModeRequest()
{

}

OSDInputModeRequest::OSDInputModeRequest(bool position, bool speed, bool inersia, bool waterspeed, bool wind, bool weather)
{
    this->position = position;
    this->speed = speed;
    this->inersia = inersia;
    this->waterspeed = waterspeed;
    this->wind = wind;
    this->weather = weather;
}

bool OSDInputModeRequest::getPosition() const
{
    return position;
}

void OSDInputModeRequest::setPosition(bool newPosition)
{
    position = newPosition;
}

bool OSDInputModeRequest::getSpeed() const
{
    return speed;
}

void OSDInputModeRequest::setSpeed(bool newSpeed)
{
    speed = newSpeed;
}

bool OSDInputModeRequest::getInersia() const
{
    return inersia;
}

void OSDInputModeRequest::setInersia(bool newInersia)
{
    inersia = newInersia;
}

bool OSDInputModeRequest::getWaterSpeed() const
{
    return waterspeed;
}

void OSDInputModeRequest::setWaterSpeed(bool newWaterSpeed)
{
    waterspeed = newWaterSpeed;
}

bool OSDInputModeRequest::getWind() const
{
    return wind;
}

void OSDInputModeRequest::setWind(bool newWind)
{
    wind = newWind;
}

bool OSDInputModeRequest::getWeather() const
{
    return weather;
}

void OSDInputModeRequest::setWeather(bool newWeather)
{
    weather = newWeather;
}

QByteArray OSDInputModeRequest::toJSON()
{
    QJsonObject obj;
    obj["position"] = position;
    obj["speed"] = speed;
    obj["inertia"] = inersia;
    obj["water_speed"] = waterspeed;
    obj["wind"] = wind;
    obj["weather"] = weather;

    return QJsonDocument(obj).toJson();
}
