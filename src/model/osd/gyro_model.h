#ifndef GYRO_MODEL_H
#define GYRO_MODEL_H

#include <QJsonObject>

class GyroModel
{
public:
    GyroModel(double heading, double pitch, double roll);
    GyroModel(){};

    static GyroModel FromJsonObject(QJsonObject obj);

    double getHeading() const;
    double getPicth() const;
    double getRoll() const;

private:
    double Heading, Pitch, Roll;
};

#endif // GYRO_MODEL_H
