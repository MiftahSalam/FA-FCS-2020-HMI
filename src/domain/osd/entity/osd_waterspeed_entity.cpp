#include "osd_waterspeed_entity.h"

OSDWaterSpeedEntity::OSDWaterSpeedEntity(double waterspeed, double watercourse, const std::string &source, const std::string &status, const OSD_MODE &mode) : OSDBaseEntity(source, status, mode),
    _speed(waterspeed),
    _course(watercourse)
{}

double OSDWaterSpeedEntity::getSpeed() const
{
    return _speed;
}

void OSDWaterSpeedEntity::setSpeed(double newWaterSpeed)
{
    _speed = newWaterSpeed;
}

double OSDWaterSpeedEntity::getCourse() const
{
    return _course;
}

void OSDWaterSpeedEntity::setCourse(double newWaterCourse)
{
    _course = newWaterCourse;
}

