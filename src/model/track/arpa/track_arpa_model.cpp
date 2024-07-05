#include "track_arpa_model.h"

TrackArpaModel::TrackArpaModel(const std::string &source, const std::string &status, int id, float range, float bearing, float speed, float course) : _source(source),
    _status(status),
    _id(id),
    _range(range),
    _bearing(bearing),
    _speed(speed),
    _course(course)
{}

std::string TrackArpaModel::source() const
{
    return _source;
}

std::string TrackArpaModel::status() const
{
    return _status;
}

int TrackArpaModel::id() const
{
    return _id;
}

float TrackArpaModel::range() const
{
    return _range;
}

float TrackArpaModel::bearing() const
{
    return _bearing;
}

float TrackArpaModel::speed() const
{
    return _speed;
}

float TrackArpaModel::course() const
{
    return _course;
}
