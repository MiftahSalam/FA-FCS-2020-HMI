#ifndef TRACKBASEENTITY_H
#define TRACKBASEENTITY_H

#include "src/shared/utils/track/track_utils.h"
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

    int getId() const;
    void setId(int newId);
    double getRange() const;
    void setRange(float newRange);
    double getBearing() const;
    void setBearing(float newBearing);
    double getSpeed() const;
    void setSpeed(float newSpeed);
    double getCourse() const;
    void setCourse(float newCourse);
    long long getTimeStamp() const;
    void setTimeStamp(long long newTimeStamp);

    TrackUtils::Identity getCurrIdentity() const;
    void setCurrIdentity(TrackUtils::Identity newCur_identity);
    TrackUtils::trackSource getCurrSource() const;
    void setCurrSource(TrackUtils::trackSource newCur_source);
    TrackUtils::Environment getCurrEnv() const;
    void setCurrEnv(TrackUtils::Environment newCur_env);
    std::string getWeaponAssign() const;
    void setWeaponAssign(const std::string &newWeapon_assign);

private:
    std::string _source;
    std::string _status;
    int _id;
    long long _timeStamp;
    double _range;
    double _bearing;
    double _speed;
    double _course;

    TrackUtils::Identity _curIdentity;
    TrackUtils::trackSource _curSource;
    TrackUtils::Environment _curEnv;
    std::string _weaponAssign;

};
#endif // TRACKBASEENTITY_H
