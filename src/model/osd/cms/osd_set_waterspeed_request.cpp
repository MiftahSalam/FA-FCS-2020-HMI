#include "osd_set_waterspeed_request.h"
#include "qjsondocument.h"
#include "qjsonobject.h"

OSDSetWaterSpeedRequest::OSDSetWaterSpeedRequest(double w_speed, double w_course): waterspeed(w_speed), watercourse(w_course)
{

}

double OSDSetWaterSpeedRequest::getWaterSpeed() const
{
    return waterspeed;
}

double OSDSetWaterSpeedRequest::getWaterCourse() const
{
    return watercourse;
}

void OSDSetWaterSpeedRequest::setWaterSpeed(const double w_speed)
{
    waterspeed = w_speed;
}

void OSDSetWaterSpeedRequest::setWaterCourse(const double w_course)
{
    watercourse = w_course;
}

QByteArray OSDSetWaterSpeedRequest::toJSON()
{
    QJsonObject obj;
    obj["waterspeed"] = waterspeed;
    obj["watercourse"] = watercourse;

    return QJsonDocument(obj).toJson();
}

