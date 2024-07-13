#include "osd_wind_entity.h"

OSDWindEntity::OSDWindEntity(double speed, double direction, const std::string &source, const std::string &status, const OSD_MODE &mode) : OSDBaseEntity(source, status, mode),
    _speed(speed),
    _direction(direction)
{}

double OSDWindEntity::getSpeed() const
{
    return _speed;
}

void OSDWindEntity::setSpeed(double newSpeed)
{
    _speed = newSpeed;
}

double OSDWindEntity::getDirection() const
{
    return _direction;
}

void OSDWindEntity::setDirection(double newDirection)
{
    _direction = newDirection;
}
