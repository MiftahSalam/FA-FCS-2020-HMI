#include "water_speed_cms_response_model.h"

WaterSpeedResponseModel::WaterSpeedResponseModel(double w_speed, double w_course): speed(w_speed), course(w_course)
{

}

WaterSpeedResponseModel WaterSpeedResponseModel::fromJsonObject(QJsonObject obj)
{
    WaterSpeedResponseModel model(
                obj.value("speed").toDouble(),
                obj.value("course").toDouble()
                );

    return model;
}

double WaterSpeedResponseModel::getSpeed() const
{
    return speed;
}

double WaterSpeedResponseModel::getCourse() const
{
    return course;
}
