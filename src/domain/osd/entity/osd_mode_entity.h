#ifndef OSD_MODE_ENTITY_H
#define OSD_MODE_ENTITY_H

#include "src/domain/osd/entity/osd_base_entity.h"

class OSDInputModeEntity
{
public:
    OSDInputModeEntity(OSD_MODE position, OSD_MODE speed, OSD_MODE inersia, OSD_MODE waterspeed, OSD_MODE wind, OSD_MODE weather);

    OSD_MODE position() const;
    void setPositionMode(OSD_MODE newPosition);
    OSD_MODE speed() const;
    void setSpeedMode(OSD_MODE newSpeed);
    OSD_MODE inersia() const;
    void setInersiaMode(OSD_MODE newInersia);
    OSD_MODE waterSpeed() const;
    void setWaterSpeedMode(OSD_MODE newWaterSpeed);
    OSD_MODE wind() const;
    void setWindMode(OSD_MODE newWind);
    OSD_MODE weather() const;
    void setWeatherMode(OSD_MODE newWeather);

private:
    OSD_MODE _position;
    OSD_MODE _speed;
    OSD_MODE _inersia;
    OSD_MODE _waterspeed;
    OSD_MODE _wind;
    OSD_MODE _weather;
};

#endif // OSD_MODE_ENTITY_H
