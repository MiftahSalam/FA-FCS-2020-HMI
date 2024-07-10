#include "osd_speed_entity.h"


OSDSpeedEntity::OSDSpeedEntity(float speed, float course, const std::string &source, const std::string &status, const OSD_MODE &mode):
    OSDBaseEntity(source,status,mode),
    _speed(speed),
    _course(course)
{

}

float OSDSpeedEntity::speed() const
{
    return _speed;
}

void OSDSpeedEntity::setSpeed(float newSpeed)
{
    _speed = newSpeed;
}

float OSDSpeedEntity::course() const
{
    return _course;
}

void OSDSpeedEntity::setCourse(float newCourse)
{
    _course = newCourse;
}

