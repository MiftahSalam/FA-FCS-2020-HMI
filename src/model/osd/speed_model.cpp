#include "speed_model.h"

SpeedModel::SpeedModel(double sog, double cog): speed(sog), course(cog)
{

}

SpeedModel SpeedModel::fromJsonObject(QJsonObject obj)
{
    SpeedModel model(
                obj.value("speed").toDouble(),
                obj.value("course").toDouble()
                );

    return model;

}

double SpeedModel::getSpeed_OG() const
{
    return speed;
}

double SpeedModel::getCourse_OG() const
{
    return course;
}
