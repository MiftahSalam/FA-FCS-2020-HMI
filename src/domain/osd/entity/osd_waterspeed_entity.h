#ifndef OSDWATERSPEEDENTITY_H
#define OSDWATERSPEEDENTITY_H

#include "osd_base_entity.h"

class OSDWaterSpeedEntity: public OSDBaseEntity
{
public:
    OSDWaterSpeedEntity(double getSpeed, double getCourse, const std::string &source, const std::string &status, const OSD_MODE &mode);

    double getSpeed() const;
    void setSpeed(double newWaterSpeed);
    double getCourse() const;
    void setCourse(double newWaterCourse);

private:
    double _speed;
    double _course;
};

#endif // OSDWATERSPEEDENTITY_H
