#include "water_speed_stream_model.h"

WaterSpeedStreamModel::WaterSpeedStreamModel() {}

WaterSpeedStreamModel::WaterSpeedStreamModel(OSDBaseModel base_data, double w_speed, double w_course)
    : speed(w_speed), course(w_course)
{
    _source = base_data.source();
    _mode = base_data.mode();
    _status = base_data.status();
}

WaterSpeedStreamModel WaterSpeedStreamModel::fromJsonObject(QJsonObject obj)
{
    OSDBaseModel base(
        obj["source"].toString().toStdString(),
        obj["status"].toString().toStdString(),
        ErrUnknown(),
        obj["mode"].toString().toStdString() == "manual" ? OSD_MODE::MANUAL : OSD_MODE::AUTO
        );

    WaterSpeedStreamModel model(
        base,
        obj.value("speed").toDouble(),
        obj.value("course").toDouble()
        );

    return model;
}

double WaterSpeedStreamModel::getSpeed() const
{
    return speed;
}

double WaterSpeedStreamModel::getCourse() const
{
    return course;
}
