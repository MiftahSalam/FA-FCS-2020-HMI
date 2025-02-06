#include "gyro_stream_model.h"

GyroStreamModel::GyroStreamModel() {}

GyroStreamModel::GyroStreamModel(OSDBaseModel base_data, double heading, double pitch, double roll)
    : heading(heading), pitch(pitch), roll(roll)
{
    _source = base_data.source();
    _mode = base_data.mode();
    _status = base_data.status();
}

GyroStreamModel GyroStreamModel::fromJsonObject(QJsonObject obj)
{
    OSDBaseModel base(
        obj["source"].toString().toStdString(),
        obj["status"].toString().toStdString(),
        ErrUnknown(),
        obj["mode"].toString().toStdString() == "manual" ? OSD_MODE::MANUAL : OSD_MODE::AUTO
        );

    GyroStreamModel model(
        base,
        obj.value("heading").toDouble(),
        obj.value("pitch").toDouble(),
        obj.value("roll").toDouble()
        );

    return model;

}

double GyroStreamModel::getHeading() const
{
    return heading;
}

double GyroStreamModel::getPitch() const
{
    return pitch;
}

double GyroStreamModel::getRoll() const
{
    return roll;
}
