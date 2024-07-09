#ifndef OSDSPEEDENTITY_H
#define OSDSPEEDENTITY_H

#include "osd_base_entity.h"

class OSDSpeedEntity: public OSDBaseEntity
{
public:
    OSDSpeedEntity(float speed, float course, const std::string &source, const std::string &status, const OSD_MODE &mode);

    float speed() const;
    void setSpeed(float newSpeed);
    float course() const;
    void setCourse(float newCourse);

private:
    float _speed;
    float _course;
};

#endif // OSDSPEEDENTITY_H
