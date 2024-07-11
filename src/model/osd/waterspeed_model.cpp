#include "waterspeed_model.h"

WaterSpeedModel::WaterSpeedModel(double w_speed, double w_course): waterspeed(w_speed), watercourse(w_course)
{

}

WaterSpeedModel WaterSpeedModel::fromJsonObject(QJsonObject obj)
{
    WaterSpeedModel model(
                obj.value("speed").toDouble(),
                obj.value("course").toDouble()
                );

    return model;
}

double WaterSpeedModel::getSpeed() const
{
    return waterspeed;
}

double WaterSpeedModel::getCourse() const
{
    return watercourse;
}
