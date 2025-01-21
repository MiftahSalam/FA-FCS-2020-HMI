#include "speed_model.h"

SpeedModel::SpeedModel(double speed, double course):
    Speed(speed), Course(course)
{

}

SpeedModel SpeedModel::FromJsonObject(QJsonObject obj)
{
    SpeedModel model(
                obj.value("sog").toDouble(),
                obj.value("cog").toDouble()
                );

    return model;
}

double SpeedModel::getSpeed() const
{
    return Speed;
}

double SpeedModel::getCourse() const
{
    return Course;
}


