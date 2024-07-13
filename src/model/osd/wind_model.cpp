#include "wind_model.h"

WindModel::WindModel(double speed, double direction): speed(speed), direction(direction)
{

}

WindModel WindModel::FromJsonObject(QJsonObject obj)
{
    WindModel model(
                obj.value("speed").toDouble(),
                obj.value("direction").toDouble()
                );
    return model;
}

double WindModel::getSpeed() const
{
    return speed;
}

double WindModel::getDirection() const
{
    return direction;
}
