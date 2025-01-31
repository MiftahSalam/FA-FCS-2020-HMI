#ifndef GYRO_MODEL_H
#define GYRO_MODEL_H

#include "src/infra/core/osd/model/osd_base_model.h"
#include <QJsonObject>

class GyroModel: public OSDBaseModel
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
