#include "wind_cms_response_model.h"

WindResponseModel::WindResponseModel(double speed, double direction): speed(speed), direction(direction)
{

}

WindResponseModel WindResponseModel::FromJsonObject(QJsonObject obj)
{
    WindResponseModel model(
                obj.value("speed").toDouble(),
                obj.value("direction").toDouble()
                );
    return model;
}

double WindResponseModel::getSpeed() const
{
    return speed;
}

double WindResponseModel::getDirection() const
{
    return direction;
}
