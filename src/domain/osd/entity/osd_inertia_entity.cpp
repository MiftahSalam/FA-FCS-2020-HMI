#include "osd_inertia_entity.h"

OSDInertiaEntity::OSDInertiaEntity(float heading, float pitch, float roll, const std::string &source, const std::string &status, const OSD_MODE &mode) : OSDBaseEntity(source, status, mode),
    _heading(heading),
    _pitch(pitch),
    _roll(roll)
{}

float OSDInertiaEntity::heading() const
{
    return _heading;
}

void OSDInertiaEntity::setHeading(float newHeading)
{
    _heading = newHeading;
}

float OSDInertiaEntity::pitch() const
{
    return _pitch;
}

void OSDInertiaEntity::setPitch(float newPitch)
{
    _pitch = newPitch;
}

float OSDInertiaEntity::roll() const
{
    return _roll;
}

void OSDInertiaEntity::setRoll(float newRoll)
{
    _roll = newRoll;
}
