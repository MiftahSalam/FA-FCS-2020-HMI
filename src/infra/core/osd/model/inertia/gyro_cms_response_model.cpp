#include "gyro_cms_response_model.h"


GyroResponseModel::GyroResponseModel(double heading, double pitch, double roll): Heading(heading), Pitch(pitch), Roll(roll)
{

}

GyroResponseModel GyroResponseModel::FromJsonObject(QJsonObject obj)
{
    GyroResponseModel model(
                obj.value("heading").toDouble(),
                obj.value("pitch").toDouble(),
                obj.value("roll").toDouble()
                );

    return model;
}

double GyroResponseModel::getHeading() const
{
    return Heading;
}

double GyroResponseModel::getPicth() const
{
    return Pitch;
}

double GyroResponseModel::getRoll() const
{
    return Roll;
}


