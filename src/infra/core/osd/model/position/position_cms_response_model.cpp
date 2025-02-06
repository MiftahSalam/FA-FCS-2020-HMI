#include "position_cms_response_model.h"

PositionResponseModel::PositionResponseModel(double lat, double lon): latitude(lat), longitude(lon)
{

}

PositionResponseModel PositionResponseModel::fromJsonObject(QJsonObject obj)
{
    PositionResponseModel model(
                obj.value("latitude").toDouble(-91),
                obj.value("latitude").toDouble(-181)
                );

    return model;
}

double PositionResponseModel::getLatitude() const
{
    return latitude;
}

double PositionResponseModel::getLongitude() const
{
    return longitude;
}
