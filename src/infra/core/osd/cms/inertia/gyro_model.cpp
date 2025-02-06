#include "gyro_model.h"


GyroModel::GyroModel(double heading, double pitch, double roll): Heading(heading), Pitch(pitch), Roll(roll)
{

}

GyroModel GyroModel::FromJsonObject(QJsonObject obj)
{
    GyroModel model(
                obj.value("heading").toDouble(),
                obj.value("pitch").toDouble(),
                obj.value("roll").toDouble()
                );

    return model;
}

double GyroModel::getHeading() const
{
    return Heading;
}

double GyroModel::getPicth() const
{
    return Pitch;
}

double GyroModel::getRoll() const
{
    return Roll;
}


