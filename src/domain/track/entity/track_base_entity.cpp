#include "track_base_entity.h"

TrackBaseEntity::TrackBaseEntity(int id, double range, double bearing, double speed, double course, const std::string &source, const std::string &status, long long timeStamp):
    _source(source),
    _status(status),
    _id(id),
    _timeStamp(timeStamp),
    _range(range),
    _bearing(bearing),
    _speed(speed),
    _course(course)
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

double TrackBaseEntity::getRange() const
{
    return _range;
}

void TrackBaseEntity::setRange(float newRange)
{
    _range = newRange;
}

double TrackBaseEntity::getBearing() const
{
    return _bearing;
}

void TrackBaseEntity::setBearing(float newBearing)
{
    _bearing = newBearing;
}

double TrackBaseEntity::getSpeed() const
{
    return _speed;
}

void TrackBaseEntity::setSpeed(float newSpeed)
{
    _speed = newSpeed;
}

double TrackBaseEntity::getCourse() const
{
    return _course;
}

void TrackBaseEntity::setCourse(float newCourse)
{
    _course = newCourse;
}

long long TrackBaseEntity::getTimeStamp() const
{
    return _timeStamp;
}

void TrackBaseEntity::setTimeStamp(long long newTimeStamp)
{
    _timeStamp = newTimeStamp;
}

TrackUtils::Identity TrackBaseEntity::getCurridentity() const
{
    return cur_identity;
}

void TrackBaseEntity::setCurrIdentity(TrackUtils::Identity newCur_identity)
{
    cur_identity = newCur_identity;
}

TrackUtils::trackSource TrackBaseEntity::getCur_source() const
{
    return cur_source;
}

void TrackBaseEntity::setCurrSource(TrackUtils::trackSource newCur_source)
{
    cur_source = newCur_source;
}

TrackUtils::Environment TrackBaseEntity::getCurrEnv() const
{
    return cur_env;
}

void TrackBaseEntity::setCurrEnv(TrackUtils::Environment newCur_env)
{
    cur_env = newCur_env;
}

std::string TrackBaseEntity::getWeaponAssign() const
{
    return weapon_assign;
}

void TrackBaseEntity::setWeaponAssign(const std::string &newWeapon_assign)
{
    weapon_assign = newWeapon_assign;
}
