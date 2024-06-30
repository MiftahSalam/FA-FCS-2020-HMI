#include "osd_set_speed_request.h"
#include "qjsondocument.h"
#include "qjsonobject.h"

OSDSetSpeedRequest::OSDSetSpeedRequest(double sog, double cog): speed(sog), course(cog)
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

void OSDSetSpeedRequest::setSpeed(const double sog)
{
    speed = sog;
}

void OSDSetSpeedRequest::setCourse(const double cog)
{
    course = cog;
}

QByteArray OSDSetSpeedRequest::toJSON()
{
    QJsonObject obj;
    obj["speed"] = speed;
    obj["course"] = course;

    return QJsonDocument(obj).toJson();
}


