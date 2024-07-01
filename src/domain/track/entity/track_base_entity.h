#ifndef TRACKBASEENTITY_H
#define TRACKBASEENTITY_H

#include <string>

class TrackBaseEntity
{
public:
    TrackBaseEntity(const std::string &source, const std::string &status);

    std::string source() const;
    void setSource(const std::string &newSource);
    std::string status() const;
    void setStatus(const std::string &newStatus);
    int getId() const;
    void setId(int newId);
    float getRange() const;
    void setRange(float newRange);
    float getBearing() const;
    void setBearing(float newBearing);
    float getSpeed() const;
    void setSpeed(float newSpeed);
    float getCourse() const;
    void setCourse(float newCourse);

private:
    std::string _source;
    std::string _status;
    int _id;
    float _range;
    float _bearing;
    float _speed;
    float _course;
};
#endif // TRACKBASEENTITY_H
