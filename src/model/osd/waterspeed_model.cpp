#include "waterspeed_model.h"

WaterSpeedModel::WaterSpeedModel(double w_speed, double w_course): waterspeed(w_speed), watercourse(w_course)
{

}

WaterSpeedModel WaterSpeedModel::fromJsonObject(QJsonObject obj)
{
    WaterSpeedModel model(
                obj.value("waterspeed").toDouble(),
                obj.value("watercourse").toDouble()
                );

    return model;
}

double WaterSpeedModel::getWaterSpeed() const
{
    return waterspeed;
}

double WaterSpeedModel::getWaterCourse() const
{
    return watercourse;
}
