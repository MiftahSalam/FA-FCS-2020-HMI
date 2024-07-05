#ifndef TRACKARPAMODEL_H
#define TRACKARPAMODEL_H

#include <QJsonObject>

class TrackArpaModel
{
public:
    TrackArpaModel(const std::string &source, const std::string &status, int id, float range, float bearing, float speed, float course);

    static TrackArpaModel fromJsonObject(QJsonObject obj);

    std::string source() const;
    std::string status() const;
    int id() const;
    float range() const;
    float bearing() const;
    float speed() const;
    float course() const;

private:
    std::string _source;
    std::string _status;
    int _id;
    float _range;
    float _bearing;
    float _speed;
    float _course;
};

#endif // TRACKARPAMODEL_H
