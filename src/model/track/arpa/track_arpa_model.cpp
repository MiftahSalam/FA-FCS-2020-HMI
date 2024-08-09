#include "track_arpa_model.h"

TrackArpaModel::TrackArpaModel(const std::string &source, const std::string &status, int id, double range, double bearing, double speed, double course):
    _source(source),
    _status(status),
    _id(id),
    _range(range),
    _bearing(bearing),
    _speed(speed),
    _course(course)
{}

TrackArpaModel TrackArpaModel::fromJsonObject(QJsonObject obj)
{
    TrackArpaModel model(
                obj.value("source").toString().toStdString(),
                obj.value("status").toString().toStdString(),
                obj.value("id").toInt(),
                obj.value("range").toDouble(),
                obj.value("bearing").toDouble(),
                obj.value("speed").toDouble(),
                obj.value("course").toDouble()
                );

    return model;
}

std::string TrackArpaModel::source() const
{
    return _source;
}

std::string TrackArpaModel::status() const
{
    return _status;
}

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
