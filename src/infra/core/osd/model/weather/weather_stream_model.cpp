#include "weather_stream_model.h"

WeatherStreamModel::WeatherStreamModel() {}

WeatherStreamModel::WeatherStreamModel(OSDBaseModel base_data, double temperature, double pressure, double humidity)
    : _temperature(temperature), _pressure(pressure), _humidity(humidity)
{
    _source = base_data.source();
    _mode = base_data.mode();
    _status = base_data.status();
}

WeatherStreamModel WeatherStreamModel::fromJsonObject(QJsonObject obj)
{
    OSDBaseModel base(
        obj["source"].toString().toStdString(),
        obj["status"].toString().toStdString(),
        ErrUnknown(),
        obj["mode"].toString().toStdString() == "manual" ? OSD_MODE::MANUAL : OSD_MODE::AUTO
        );

    WeatherStreamModel model(
        base,
        obj.value("temperature").toDouble(),
        obj.value("pressure").toDouble(),
        obj.value("humidity").toDouble()
        );

    return model;
}

double WeatherStreamModel::getTemperature() const
{
    return _temperature;
}

double WeatherStreamModel::getPressure() const
{
    return _pressure;
}

double WeatherStreamModel::getHumidity() const
{
    return _humidity;
}
