#include "track_base_entity.h"

TrackBaseEntity::TrackBaseEntity(const std::string &source, const std::string &status)
    : _source(source), _status(status)
{}

std::string TrackBaseEntity::source() const
{
    return _source;
}

void TrackBaseEntity::setSource(const std::string &newSource)
{
    _source = newSource;
}

std::string TrackBaseEntity::status() const
{
    return _status;
}

void TrackBaseEntity::setStatus(const std::string &newStatus)
{
    _status = newStatus;
}

int TrackBaseEntity::getId() const
{
    return _id;
}

void TrackBaseEntity::setId(int newId)
{
    _id = newId;
}

float TrackBaseEntity::getRange() const
{
    return _range;
}

void TrackBaseEntity::setRange(float newRange)
{
    _range = newRange;
}

float TrackBaseEntity::getBearing() const
{
    return _bearing;
}

void TrackBaseEntity::setBearing(float newBearing)
{
    _bearing = newBearing;
}

float TrackBaseEntity::getSpeed() const
{
    return _speed;
}

void TrackBaseEntity::setSpeed(float newSpeed)
{
    _speed = newSpeed;
}

float TrackBaseEntity::getCourse() const
{
    return _course;
}

void TrackBaseEntity::setCourse(float newCourse)
{
    _course = newCourse;
}
