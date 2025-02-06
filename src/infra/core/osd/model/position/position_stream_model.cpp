#include "position_stream_model.h"

PositionStreamModel::PositionStreamModel() {}

PositionStreamModel::PositionStreamModel(OSDBaseModel base_data, double lat, double lon)
    : latitude(lat), longitude(lon)
{
    _source = base_data.source();
    _mode = base_data.mode();
    _status = base_data.status();
}

PositionStreamModel PositionStreamModel::fromJsonObject(QJsonObject obj)
{
    OSDBaseModel base(
        obj["source"].toString().toStdString(),
        obj["status"].toString().toStdString(),
        ErrUnknown(),
        obj["status"].toString().toStdString() == "manual" ? OSD_MODE::MANUAL : OSD_MODE::AUTO
        );

    PositionStreamModel model(
        base,
        obj.value("latitude").toDouble(-91),
        obj.value("latitude").toDouble(-181)
        );

    return model;
}

double PositionStreamModel::getLatitude() const
{
    return latitude;
}

double PositionStreamModel::getLongitude() const
{
    return longitude;
}
