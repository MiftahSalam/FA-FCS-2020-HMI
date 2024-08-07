#include "track_base_entity.h"

TrackBaseEntity::TrackBaseEntity(int id, double range, double bearing, double speed, double course, const std::string &source, const std::string &status):
    _id(id),
    _range(range),
    _bearing(bearing),
    _speed(speed),
    _course(course),
    _source(source),
    _status(status)
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

int TrackBaseEntity::Id() const
{
    return _id;
}

void TrackBaseEntity::setId(int newId)
{
    _id = newId;
}

double TrackBaseEntity::range() const
{
    return _range;
}

void TrackBaseEntity::setRange(float newRange)
{
    _range = newRange;
}

double TrackBaseEntity::bearing() const
{
    return _bearing;
}

void TrackBaseEntity::setBearing(float newBearing)
{
    _bearing = newBearing;
}

double TrackBaseEntity::speed() const
{
    return _speed;
}

void TrackBaseEntity::setSpeed(float newSpeed)
{
    _speed = newSpeed;
}

double TrackBaseEntity::course() const
{
    return _course;
}

void TrackBaseEntity::setCourse(float newCourse)
{
    _course = newCourse;
}
