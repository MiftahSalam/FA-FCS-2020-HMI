#include "osd_set_waterspeed_request.h"
#include "qjsondocument.h"
#include "qjsonobject.h"

OSDSetWaterSpeedRequest::OSDSetWaterSpeedRequest(double w_speed, double w_course): speed(w_speed), course(w_course)
{

}

double OSDSetWaterSpeedRequest::getSpeed() const
{
    return speed;
}

double OSDSetWaterSpeedRequest::getCourse() const
{
    return course;
}

void OSDSetWaterSpeedRequest::setSpeed(const double w_speed)
{
    speed = w_speed;
}

void OSDSetWaterSpeedRequest::setCourse(const double w_course)
{
    course = w_course;
}

QByteArray OSDSetWaterSpeedRequest::toJSON()
{
    QJsonObject obj;
    obj["waterspeed"] = speed;
    obj["watercourse"] = course;

    return QJsonDocument(obj).toJson();
}

