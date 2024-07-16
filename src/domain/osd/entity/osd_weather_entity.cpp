#include "osd_weather_entity.h"

OSDWeatherEntity::OSDWeatherEntity(float temp, float hum, float press, const std::string &source, const std::string &status, const OSD_MODE &mode) : OSDBaseEntity(source, status, mode),
    _temp(temp),
    _hum(hum),
    _press(press)
{}

float OSDWeatherEntity::temp() const
{
    return _temp;
}

void OSDWeatherEntity::setTemp(float newTemp)
{
    _temp = newTemp;
}

float OSDWeatherEntity::hum() const
{
    return _hum;
}

void OSDWeatherEntity::setHum(float newHum)
{
    _hum = newHum;
}

float OSDWeatherEntity::press() const
{
    return _press;
}

void OSDWeatherEntity::setPress(float newPress)
{
    _press = newPress;
}

