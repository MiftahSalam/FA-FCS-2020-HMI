#include "osd_set_position_request.h"
#include "qjsondocument.h"
#include "qjsonobject.h"

OSDSetPositionRequest::OSDSetPositionRequest(double lat, double lon): latitude(lat), longitude(lon)
{

}

double OSDSetPositionRequest::getLatitude() const
{
    return latitude;
}

double OSDSetPositionRequest::getLongitude() const
{
    return longitude;
}

void OSDSetPositionRequest::setLatitude(const double lat)
{
    latitude = lat;
}

void OSDSetPositionRequest::setLongitude(const double lon)
{
    longitude = lon;
}


QByteArray OSDSetPositionRequest::toJSON()
{
    QJsonObject obj;
    obj["latitude"] = latitude;
    obj["longitude"] = longitude;

    return QJsonDocument(obj).toJson();
}
