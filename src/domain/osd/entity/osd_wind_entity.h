#ifndef OSDWINDENTITY_H
#define OSDWINDENTITY_H
#include "osd_base_entity.h"

class OSDWindEntity: public OSDBaseEntity
{
public:
    OSDWindEntity(double getSpeed, double getDirection, const std::string &source, const std::string &status, const OSD_MODE &mode);

    double getSpeed() const;
    void setSpeed(double newSpeed);
    double getDirection() const;
    void setDirection(double newDirection);

private:
    double _speed;
    double _direction;
};

#endif // OSDWINDENTITY_H
