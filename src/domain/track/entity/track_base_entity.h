#ifndef TRACKBASEENTITY_H
#define TRACKBASEENTITY_H

#include <string>

class TrackBaseEntity
{
public:
    TrackBaseEntity(
        int id,
        double range,
        double bearing,
        double speed,
        double course,
        const std::string &source,
        const std::string &status,
        long long timeStamp
        );

    std::string source() const;
    void setSource(const std::string &newSource);
    std::string status() const;
    void setStatus(const std::string &newStatus);

    int Id() const;
    void setId(int newId);
    double range() const;
    void setRange(float newRange);
    double bearing() const;
    void setBearing(float newBearing);
    double speed() const;
    void setSpeed(float newSpeed);
    double course() const;
    void setCourse(float newCourse);
    long long timeStamp() const;
    void setTimeStamp(long long newTimeStamp);

private:
    std::string _source;
    std::string _status;
    int _id;
    double _range;
    double _bearing;
    double _speed;
    double _course;
    long long _timeStamp;
};
#endif // TRACKBASEENTITY_H
