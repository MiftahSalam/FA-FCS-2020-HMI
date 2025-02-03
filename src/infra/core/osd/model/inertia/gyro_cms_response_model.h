#ifndef GYRO_CMS_RESPONSE_MODEL_H
#define GYRO_CMS_RESPONSE_MODEL_H

#include "src/infra/core/osd/model/osd_base_model.h"
#include <QJsonObject>

class GyroResponseModel: public OSDBaseModel
{
public:
    GyroResponseModel(double heading, double pitch, double roll);
    GyroResponseModel(){};

    static GyroResponseModel FromJsonObject(QJsonObject obj);

    double getHeading() const;
    double getPicth() const;
    double getRoll() const;

private:
    double Heading, Pitch, Roll;
};

#endif // GYRO_CMS_RESPONSE_MODEL_H
