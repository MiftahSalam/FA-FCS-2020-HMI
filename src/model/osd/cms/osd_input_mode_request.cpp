#include "osd_input_mode_request.h"
#include "qjsondocument.h"
#include "qjsonobject.h"

OSDInputModeRequest::OSDInputModeRequest()
{

}

OSDInputModeRequest::OSDInputModeRequest(bool position, bool speed, bool inersia)
{
    this->position = position;
    this->speed = speed;
    this->inersia = inersia;
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


QByteArray OSDInputModeRequest::toJSON()
{
    QJsonObject obj;
    obj["position"] = position;
    obj["speed"] = speed;
    obj["inertia"] = inersia;

    return QJsonDocument(obj).toJson();
}
