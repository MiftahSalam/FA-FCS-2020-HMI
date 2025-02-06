#include "position_model.h"

PositionModel::PositionModel(double lat, double lon): latitude(lat), longitude(lon)
{

}

PositionModel PositionModel::fromJsonObject(QJsonObject obj)
{
    PositionModel model(
                obj.value("latitude").toDouble(-91),
                obj.value("latitude").toDouble(-181)
                );

    return model;
}

double PositionModel::getLatitude() const
{
    return latitude;
}

double PositionModel::getLongitude() const
{
    return longitude;
}
