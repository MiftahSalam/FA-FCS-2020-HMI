#ifndef SPEED_STREAM_MODEL_H
#define SPEED_STREAM_MODEL_H

#include "qjsonobject.h"

#include "src/infra/core/osd/model/osd_base_model.h"

class SpeedStreamModel: public OSDBaseModel
{
public:
    SpeedStreamModel();
    SpeedStreamModel(OSDBaseModel base_data, double speed, double course);

    static SpeedStreamModel fromJsonObject(QJsonObject obj);

    double getSpeed() const;
    double getCourse() const;

private:
    double speed, course;
};

#endif // SPEED_STREAM_MODEL_H
