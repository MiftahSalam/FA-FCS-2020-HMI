#ifndef WATER_SPEED_STREAM_MODEL_H
#define WATER_SPEED_STREAM_MODEL_H

#include "qjsonobject.h"

#include "src/infra/core/osd/model/osd_base_model.h"

class WaterSpeedStreamModel: public OSDBaseModel
{
public:
    WaterSpeedStreamModel();
    WaterSpeedStreamModel(OSDBaseModel base_data, double w_speed, double w_course);

    static WaterSpeedStreamModel fromJsonObject(QJsonObject obj);

    double getSpeed() const;
    double getCourse() const;

private:
    double speed, course;
};

#endif // WATER_SPEED_STREAM_MODEL_H
