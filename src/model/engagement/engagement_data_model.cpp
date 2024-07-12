#include "engagement_data_model.h"

EngagementDataModel::EngagementDataModel()
{

}

EngagementDataModel::EngagementDataModel(const std::string &weapon, float azimuth, float elevation, int status) : _weapon(weapon),
    _azimuth(azimuth),
    _elevation(elevation),
    status(status)
{}

EngagementDataModel EngagementDataModel::fromJsonObject(QJsonObject obj)
{

}

std::string EngagementDataModel::weapon() const
{
    return _weapon;
}

float EngagementDataModel::azimuth() const
{
    return _azimuth;
}

float EngagementDataModel::elevation() const
{
    return _elevation;
}

int EngagementDataModel::getStatus() const
{
    return status;
}
