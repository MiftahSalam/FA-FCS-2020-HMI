#include "track_param.h"

TrackParam::TrackParam(int tn, double range, double bearing, double height, double speed, double course, TrackUtils::Identity cur_identity, TrackUtils::trackSource cur_source, TrackUtils::Environment cur_env, const QString &weapon_assign) : tn(tn),
    range(range),
    bearing(bearing),
    height(height),
    speed(speed),
    course(course),
    cur_identity(cur_identity),
    cur_source(cur_source),
    cur_env(cur_env),
    weapon_assign(weapon_assign)
{}

TrackParam::TrackParam(const TrackParam &track)
{
    range = track.getRange();
    bearing = track.getRange();
    height = track.getRange();
    speed = track.getRange();
    course = track.getRange();
    cur_identity = track.getCur_identity();
    cur_source = track.getCur_source();
    cur_env = track.getCur_env();
    weapon_assign = track.getWeapon_assign();
}

void TrackParam::operator=(const TrackParam &track)
{
    range = track.getRange();
    bearing = track.getRange();
    height = track.getRange();
    speed = track.getRange();
    course = track.getRange();
    cur_identity = track.getCur_identity();
    cur_source = track.getCur_source();
    cur_env = track.getCur_env();
    weapon_assign = track.getWeapon_assign();
}

int TrackParam::getTn() const
{
    return tn;
}

double TrackParam::getRange() const
{
    return range;
}

double TrackParam::getBearing() const
{
    return bearing;
}

double TrackParam::getHeight() const
{
    return height;
}

double TrackParam::getSpeed() const
{
    return speed;
}

double TrackParam::getCourse() const
{
    return course;
}

TrackUtils::Identity TrackParam::getCur_identity() const
{
    return cur_identity;
}

TrackUtils::trackSource TrackParam::getCur_source() const
{
    return cur_source;
}

TrackUtils::Environment TrackParam::getCur_env() const
{
    return cur_env;
}

QString TrackParam::getWeapon_assign() const
{
    return weapon_assign;
}
