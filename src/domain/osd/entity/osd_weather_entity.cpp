#include "osd_weather_entity.h"

OSDWeatherEntity::OSDWeatherEntity(float temp, float hum, float press, const std::string &source, const std::string &status, const OSD_MODE &mode) : OSDBaseEntity(source, status, mode),
    _temperature(temp),
    _humidity(hum),
    _pressure(press)
{}

float OSDWeatherEntity::temperature() const
{
    return _temperature;
}

void OSDWeatherEntity::setTemperature(float newTemp)
{
    _temperature = newTemp;
}

float OSDWeatherEntity::humidity() const
{
    return _humidity;
}

void OSDWeatherEntity::setHumidity(float newHum)
{
    _humidity = newHum;
}

float OSDWeatherEntity::pressure() const
{
    return _pressure;
}

void OSDWeatherEntity::setPressure(float newPress)
{
    _pressure = newPress;
}

