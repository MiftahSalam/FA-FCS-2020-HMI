#include "date_time_stream_model.h"

DateTimeStreamModel::DateTimeStreamModel() {}

DateTimeStreamModel::DateTimeStreamModel(OSDBaseModel base_data, QString local, QString utc)
    : _dateTimeLocal(local), _dateTimeUTC(utc)
{
    _source = base_data.source();
    _mode = base_data.mode();
    _status = base_data.status();
}

DateTimeStreamModel DateTimeStreamModel::fromJsonObject(QJsonObject obj)
{
    OSDBaseModel base(
        obj["source"].toString().toStdString(),
        obj["status"].toString().toStdString(),
        ErrUnknown(),
        obj["mode"].toString().toStdString() == "manual" ? OSD_MODE::MANUAL : OSD_MODE::AUTO
        );

    DateTimeStreamModel model(
        base,
        obj.value("date_time_local").toString(),
        obj.value("date_time_utc").toString()
        );

    return model;
}

QString DateTimeStreamModel::getDateTimeLocal() const
{
    return _dateTimeLocal;
}

QString DateTimeStreamModel::getDateTimeUTC() const
{
    return _dateTimeUTC;
}
