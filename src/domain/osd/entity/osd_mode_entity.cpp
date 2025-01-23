#include "osd_mode_entity.h"

OSDInputModeEntity::OSDInputModeEntity(bool position, bool speed, bool inersia, bool waterspeed, bool wind, bool weather)
    : _position(position),_speed(speed),_inersia(inersia),_waterspeed(waterspeed),_wind(wind),_weather(weather)
{

}

bool OSDInputModeEntity::position() const
{
    return _position;
}

void OSDInputModeEntity::setPositionMode(bool newPosition)
{
    _position = newPosition;
}

bool OSDInputModeEntity::speed() const
{
    return _speed;
}

void OSDInputModeEntity::setSpeedMode(bool newSpeed)
{
    _speed = newSpeed;
}

bool OSDInputModeEntity::inersia() const
{
    return _inersia;
}

void OSDInputModeEntity::setInersiaMode(bool newInersia)
{
    _inersia = newInersia;
}

bool OSDInputModeEntity::waterSpeed() const
{
    return _waterspeed;
}

void OSDInputModeEntity::setWaterSpeedMode(bool newWaterSpeed)
{
    _waterspeed = newWaterSpeed;
}

bool OSDInputModeEntity::wind() const
{
    return _wind;
}

void OSDInputModeEntity::setWindMode(bool newWind)
{
    _wind = newWind;
}

bool OSDInputModeEntity::weather() const
{
    return _weather;
}

void OSDInputModeEntity::setWeatherMode(bool newWeather)
{
    _weather = newWeather;
}
