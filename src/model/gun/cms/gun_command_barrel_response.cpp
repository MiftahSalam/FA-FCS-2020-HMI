#include "gun_command_barrel_response.h"

GunCommandBarrelResponse::GunCommandBarrelResponse()
{

}

float GunCommandBarrelResponse::getAzimuth() const
{
    return azimuth;
}

float GunCommandBarrelResponse::getElevation() const
{
    return elevation;
}

GunCommandBarrelResponse::GunCommandBarrelResponse(float azimuth, float elevation) : azimuth(azimuth),
    elevation(elevation)
{}

GunCommandBarrelResponse GunCommandBarrelResponse::FromJsonObject(QJsonObject obj)
{
    GunCommandBarrelResponse model(
                obj.value("azimuth").toDouble(),
                obj.value("elevation").toDouble()
                );

    return model;
}
