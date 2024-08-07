#include "track_arpa_model.h"

TrackArpaModel::TrackArpaModel(int id, double range, double bearing, double speed, double course):
    _id(id),
    _range(range),
    _bearing(bearing),
    _speed(speed),
    _course(course)
{}

int TrackArpaModel::getId() const
{
    return _id;
}

double TrackArpaModel::getRange() const
{
    return _range;
}

double TrackArpaModel::getBearing() const
{
    return _bearing;
}

double TrackArpaModel::getSpeed() const
{
    return _speed;
}

double TrackArpaModel::getCourse() const
{
    return _course;
}
