#include "speed_stream_model.h"

SpeedStreamModel::SpeedStreamModel() {}

SpeedStreamModel::SpeedStreamModel(OSDBaseModel base_data, double speed, double course)
: speed(speed), course(course)
{
    _source = base_data.source();
    _mode = base_data.mode();
    _status = base_data.status();
}

SpeedStreamModel SpeedStreamModel::fromJsonObject(QJsonObject obj)
{
    OSDBaseModel base(
        obj["source"].toString().toStdString(),
        obj["status"].toString().toStdString(),
        ErrUnknown(),
        obj["mode"].toString().toStdString() == "manual" ? OSD_MODE::MANUAL : OSD_MODE::AUTO
        );

    SpeedStreamModel model(
        base,
        obj.value("sog").toDouble(),
        obj.value("cog").toDouble()
        );

    return model;
}

double SpeedStreamModel::getSpeed() const
{
    return speed;
}

double SpeedStreamModel::getCourse() const
{
    return course;
}
