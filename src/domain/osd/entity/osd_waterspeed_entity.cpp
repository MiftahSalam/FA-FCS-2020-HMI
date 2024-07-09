#include "osd_waterspeed_entity.h"

OSDWaterSpeedEntity::OSDWaterSpeedEntity(double waterspeed, double watercourse, const std::string &source, const std::string &status, const OSD_MODE &mode) : OSDBaseEntity(source, status, mode),
    _waterspeed(waterspeed),
    _watercourse(watercourse)
{}

double OSDWaterSpeedEntity::waterspeed() const
{
    return _waterspeed;
}

void OSDWaterSpeedEntity::setWaterSpeed(double newWaterSpeed)
{
    _waterspeed = newWaterSpeed;
}

double OSDWaterSpeedEntity::watercourse() const
{
    return _watercourse;
}

void OSDWaterSpeedEntity::setWaterCourse(double newWaterCourse)
{
    _watercourse = newWaterCourse;
}

