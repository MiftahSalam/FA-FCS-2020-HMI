#include "gun_barrel_model.h"

GunBarrelModel::GunBarrelModel(double azimuth, double elevation):
    _azimuth(azimuth), _elevation(elevation)
{}

GunBarrelModel GunBarrelModel::fromJsonObject(QJsonObject obj)
{
    GunBarrelModel model(
        obj.value("azimuth").toDouble(),
        obj.value("elevation").toDouble()
        );
    return model;
}

double GunBarrelModel::getAzimuth() const
{
    return _azimuth;
}

double GunBarrelModel::getElevation() const
{
    return _elevation;
}
