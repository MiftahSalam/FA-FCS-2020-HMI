#include "osd_set_position_reqeust.h"

OSDSetPositionReqeust::OSDSetPositionReqeust(double lat, double lon): latitude(lat), longitude(lon)
{

}

double OSDSetPositionReqeust::getLatitude() const
{
    return longitude;
}

double OSDSetPositionReqeust::getLongitude() const
{
    return latitude;
}

void OSDSetPositionReqeust::setLatitude(const double lat)
{
    latitude = lat;
}

void OSDSetPositionReqeust::setLongitude(const double lon)
{
    longitude = lon;
}
