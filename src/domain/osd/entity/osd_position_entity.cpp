#include "osd_position_entity.h"

OSDPositionEntity::OSDPositionEntity(double latitude, double longitude, const std::string &source, const std::string &status, const OSD_MODE &mode) : OSDBaseEntity(source, status, mode),
    _latitude(latitude),
    _longitude(longitude)
{}

double OSDPositionEntity::latitude() const
{
    return _latitude;
}

void OSDPositionEntity::setLatitude(double newLatitude)
{
    _latitude = newLatitude;
}

double OSDPositionEntity::longitude() const
{
    return _longitude;
}

void OSDPositionEntity::setLongitude(double newLongitude)
{
    _longitude = newLongitude;
}
