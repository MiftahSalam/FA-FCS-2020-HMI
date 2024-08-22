#include "gun_command_barrel_request.h"
#include "qjsondocument.h"
#include "qjsonobject.h"

GunCommandBarrelRequest::GunCommandBarrelRequest()
{

}


QByteArray GunCommandBarrelRequest::toJSON()
{
    QJsonObject obj;
    obj["azimuth"] = azimuth;
    obj["elevation"] = elevation;

    return QJsonDocument(obj).toJson();
}

float GunCommandBarrelRequest::getAzimuth() const
{
    return azimuth;
}

void GunCommandBarrelRequest::setAzimuth(float newAzimuth)
{
    azimuth = newAzimuth;
}

float GunCommandBarrelRequest::getElevation() const
{
    return elevation;
}

void GunCommandBarrelRequest::setElevation(float newElevation)
{
    elevation = newElevation;
}

GunCommandBarrelRequest::GunCommandBarrelRequest(float azimuth, float elevation) : azimuth(azimuth),
    elevation(elevation)
{}
