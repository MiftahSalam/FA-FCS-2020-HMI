#include "osd_set_wind_request.h"
#include "qjsondocument.h"
#include "qjsonobject.h"

OSDSetWindRequest::OSDSetWindRequest(double _speed, double _direction): speed(_speed), direction(_direction)
{

}

double OSDSetWindRequest::getSpeed() const
{
    return speed;
}

double OSDSetWindRequest::getDirection() const
{
    return direction;
}

void OSDSetWindRequest::setSpeed(const double _speed)
{
    speed = _speed;
}

void OSDSetWindRequest::setDirection(const double _direction)
{
    direction = _direction;
}

QByteArray OSDSetWindRequest::toJSON()
{
    QJsonObject obj;
    obj["speed"] = speed;
    obj["direction"] = direction;

    return QJsonDocument(obj).toJson();
}

