#ifndef OSDINERTIAENTITY_H
#define OSDINERTIAENTITY_H

#include "osd_base_entity.h"

class OSDInertiaEntity: public OSDBaseEntity
{
public:
    OSDInertiaEntity(float heading, float pitch, float roll, const std::string &source, const std::string &status, const OSD_MODE &mode);

    float heading() const;
    void setHeading(float newHeading);
    float pitch() const;
    void setPitch(float newPitch);
    float roll() const;
    void setRoll(float newRoll);

private:
    float _heading;
    float _pitch;
    float _roll;
};

#endif // OSDINERTIAENTITY_H
