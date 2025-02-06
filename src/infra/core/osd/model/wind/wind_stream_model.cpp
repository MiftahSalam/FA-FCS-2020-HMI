#include "wind_stream_model.h"

WindStreamModel::WindStreamModel() {}

WindStreamModel::WindStreamModel(OSDBaseModel base_data, double speed, double course)
    : _speed(speed), _course(course)
{
    _source = base_data.source();
    _mode = base_data.mode();
    _status = base_data.status();
}

WindStreamModel WindStreamModel::fromJsonObject(QJsonObject obj)
{
    OSDBaseModel base(
        obj["source"].toString().toStdString(),
        obj["status"].toString().toStdString(),
        ErrUnknown(),
        obj["mode"].toString().toStdString() == "manual" ? OSD_MODE::MANUAL : OSD_MODE::AUTO
        );

    WindStreamModel model(
        base,
        obj.value("speed").toDouble(),
        obj.value("course").toDouble()
        );

    return model;
}

double WindStreamModel::getSpeed() const
{
    return _speed;
}

double WindStreamModel::getDirection() const
{
    return _course;
}
