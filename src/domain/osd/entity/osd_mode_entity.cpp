#include "osd_mode_entity.h"

OSDInputModeEntity::OSDInputModeEntity(OSD_MODE position, OSD_MODE speed, OSD_MODE inersia, OSD_MODE waterspeed, OSD_MODE wind, OSD_MODE weather)
    : _position(position),_speed(speed),_inersia(inersia),_waterspeed(waterspeed),_wind(wind),_weather(weather)
{

}

OSD_MODE OSDInputModeEntity::position() const
{
    return _position;
}

void OSDInputModeEntity::setPositionMode(OSD_MODE newPosition)
{
    _position = newPosition;
}

OSD_MODE OSDInputModeEntity::speed() const
{
    return _speed;
}

void OSDInputModeEntity::setSpeedMode(OSD_MODE newSpeed)
{
    _speed = newSpeed;
}

OSD_MODE OSDInputModeEntity::inersia() const
{
    return _inersia;
}

void OSDInputModeEntity::setInersiaMode(OSD_MODE newInersia)
{
    _inersia = newInersia;
}

OSD_MODE OSDInputModeEntity::waterSpeed() const
{
    return _waterspeed;
}

void OSDInputModeEntity::setWaterSpeedMode(OSD_MODE newWaterSpeed)
{
    _waterspeed = newWaterSpeed;
}

OSD_MODE OSDInputModeEntity::wind() const
{
    return _wind;
}

void OSDInputModeEntity::setWindMode(OSD_MODE newWind)
{
    _wind = newWind;
}

OSD_MODE OSDInputModeEntity::weather() const
{
    return _weather;
}

void OSDInputModeEntity::setWeatherMode(OSD_MODE newWeather)
{
    _weather = newWeather;
}
