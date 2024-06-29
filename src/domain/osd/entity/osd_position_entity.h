#ifndef OSDPOSITIONENTITY_H
#define OSDPOSITIONENTITY_H

#include "osd_base_entity.h"

class OSDPositionEntity: public OSDBaseEntity
{
public:
    OSDPositionEntity(double latitude, double longitude, const std::string &source, const std::string &status, const OSD_MODE &mode);

    double latitude() const;
    void setLatitude(double newLatitude);
    void resetLatitude();
    double longitude() const;
    void setLongitude(double newLongitude);
    void resetLongitude();

private:
    double _latitude;
    double _longitude;
};

#endif // OSDPOSITIONENTITY_H
