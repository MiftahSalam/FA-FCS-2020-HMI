#include "osd_waterspeed_entity.h"

OSDWaterSpeedEntity::OSDWaterSpeedEntity(double waterspeed, double watercourse, const std::string &source, const std::string &status, const OSD_MODE &mode) : OSDBaseEntity(source, status, mode),
    _speed(waterspeed),
    _course(watercourse)
{}

double OSDWaterSpeedEntity::waterspeed() const
{
    return _speed;
}

void OSDWaterSpeedEntity::setWaterSpeed(double newWaterSpeed)
{
    _speed = newWaterSpeed;
}

double OSDWaterSpeedEntity::watercourse() const
{
    return _course;
}

void OSDWaterSpeedEntity::setWaterCourse(double newWaterCourse)
{
    _course = newWaterCourse;
}

