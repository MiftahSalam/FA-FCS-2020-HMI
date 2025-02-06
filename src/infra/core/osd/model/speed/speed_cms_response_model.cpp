#include "speed_cms_response_model.h"

SpeedResponseModel::SpeedResponseModel(double speed, double course):
    Speed(speed), Course(course)
{

}

SpeedResponseModel SpeedResponseModel::FromJsonObject(QJsonObject obj)
{
    SpeedResponseModel model(
                obj.value("sog").toDouble(),
                obj.value("cog").toDouble()
                );

    return model;
}

double SpeedResponseModel::getSpeed() const
{
    return Speed;
}

double SpeedResponseModel::getCourse() const
{
    return Course;
}


