#ifndef WIND_STREAM_MODEL_H
#define WIND_STREAM_MODEL_H

#include "qjsonobject.h"

#include "src/infra/core/osd/model/osd_base_model.h"

class WindStreamModel: public OSDBaseModel
{
public:
    WindStreamModel();
    WindStreamModel(OSDBaseModel base_data, double speed, double course);

    static WindStreamModel fromJsonObject(QJsonObject obj);

    double getSpeed() const;
    double getDirection() const;

private:
    double _speed, _course;
};

#endif // WIND_STREAM_MODEL_H
