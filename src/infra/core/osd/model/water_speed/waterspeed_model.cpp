#include "waterspeed_model.h"

WaterSpeedModel::WaterSpeedModel(double w_speed, double w_course): speed(w_speed), course(w_course)
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
    return speed;
}

double WaterSpeedModel::getCourse() const
{
    return course;
}
