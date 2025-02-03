#ifndef GYRO_STREAM_MODEL_H
#define GYRO_STREAM_MODEL_H

#include "qjsonobject.h"

#include "src/infra/core/osd/model/osd_base_model.h"

class GyroStreamModel: public OSDBaseModel
{
public:
    GyroStreamModel();
    GyroStreamModel(OSDBaseModel base_data, double heading, double pitch, double roll);

    static GyroStreamModel fromJsonObject(QJsonObject obj);

    double getHeading() const;
    double getPitch() const;
    double getRoll() const;

private:
    double heading;
    double pitch;
    double roll;
};

#endif // GYRO_STREAM_MODEL_H
