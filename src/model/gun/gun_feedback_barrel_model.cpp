#include "gun_feedback_barrel_model.h"

GunFeedbackBarrelModel::GunFeedbackBarrelModel(double azimuth, double elevation):
    _azimuth(azimuth), _elevation(elevation)
{}

GunFeedbackBarrelModel GunFeedbackBarrelModel::fromJsonObject(QJsonObject obj)
{
    GunFeedbackBarrelModel model(
        obj.value("azimuth").toDouble(),
        obj.value("elevation").toDouble()
        );
    return model;
}

double GunFeedbackBarrelModel::getAzimuth() const
{
    return _azimuth;
}

double GunFeedbackBarrelModel::getElevation() const
{
    return _elevation;
}
