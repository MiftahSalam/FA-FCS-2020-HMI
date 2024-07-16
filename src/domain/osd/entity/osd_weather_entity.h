#ifndef OSD_WEATHER_ENTITY_H
#define OSD_WEATHER_ENTITY_H

#include "osd_base_entity.h"

class OSDWeatherEntity: public OSDBaseEntity
{
public:
    OSDWeatherEntity(float temp, float hum, float press, const std::string &source, const std::string &status, const OSD_MODE &mode);

    float temp() const;
    void setTemp(float newTemp);
    float hum() const;
    void setHum(float newHum);
    float press() const;
    void setPress(float newPress);

private:
    float _temp;
    float _hum;
    float _press;
};


#endif // OSD_WEATHER_ENTITY_H
