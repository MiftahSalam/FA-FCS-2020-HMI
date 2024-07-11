#ifndef OSDWATERSPEEDENTITY_H
#define OSDWATERSPEEDENTITY_H

#include "osd_base_entity.h"

class OSDWaterSpeedEntity: public OSDBaseEntity
{
public:
    OSDWaterSpeedEntity(double waterspeed, double watercourse, const std::string &source, const std::string &status, const OSD_MODE &mode);

    double waterspeed() const;
    void setWaterSpeed(double newWaterSpeed);
    double watercourse() const;
    void setWaterCourse(double newWaterCourse);

private:
    double _speed;
    double _course;
};

#endif // OSDWATERSPEEDENTITY_H
