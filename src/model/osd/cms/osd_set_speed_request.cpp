#include "osd_set_speed_request.h"
#include "qjsondocument.h"
#include "qjsonobject.h"

OSDSetSpeedRequest::OSDSetSpeedRequest(double _speed, double _course):
    speed(_speed), course(_course)
{

}

double OSDSetSpeedRequest::getSpeed() const
{
    return speed;
}

double OSDSetSpeedRequest::getCourse() const
{
    return course;
}

void OSDSetSpeedRequest::setSpeed(const double _sog)
{
    speed = _sog;
}

void OSDSetSpeedRequest::setCourse(const double _cog)
{
    course = _cog;
}

QByteArray OSDSetSpeedRequest::toJSON()
{
    QJsonObject obj;
    obj["speed"] = speed;
    obj["course"] = course;

    return QJsonDocument(obj).toJson();
}


