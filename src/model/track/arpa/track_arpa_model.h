#ifndef TRACKARPAMODEL_H
#define TRACKARPAMODEL_H

#include <QJsonObject>

class TrackArpaModel
{
public:
    // TrackArpaModel(const std::string &source, const std::string &status, int id, float range, float bearing, float speed, float course);
    TrackArpaModel(const std::string &source, const std::string &status, int id, double range, double bearing, double speed, double course);
    TrackArpaModel(){}

    static TrackArpaModel fromJsonObject(QJsonObject obj);

    std::string source() const;
    std::string status() const;
    int getId() const;
    double getRange() const;
    double getBearing() const;
    double getSpeed() const;
    double getCourse() const;

private:
    std::string _source;
    std::string _status;
    int _id;
    double _range;
    double _bearing;
    double _speed;
    double _course;
};

#endif // TRACKARPAMODEL_H
