#include "osd_input_mode_request.h"
#include "qjsondocument.h"
#include "qjsonobject.h"

OSDInputModeRequest::OSDInputModeRequest()
{

}

OSDInputModeRequest::OSDInputModeRequest(bool position, bool speed, bool inersia, bool waterspeed)
{
    this->position = position;
    this->speed = speed;
    this->inersia = inersia;
    this->waterspeed = waterspeed;
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
QByteArray OSDInputModeRequest::toJSON()
{
    QJsonObject obj;
    obj["position"] = position;
    obj["speed"] = speed;
    obj["inertia"] = inersia;
    obj["waterspeed"] = waterspeed;

    return QJsonDocument(obj).toJson();
}
