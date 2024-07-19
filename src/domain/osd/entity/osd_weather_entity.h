#ifndef OSD_WEATHER_ENTITY_H
#define OSD_WEATHER_ENTITY_H

#include "osd_base_entity.h"

class OSDWeatherEntity: public OSDBaseEntity
{
public:
    OSDWeatherEntity(float temp, float press, float hum, const std::string &source, const std::string &status, const OSD_MODE &mode);

    float temperature() const;
    void setTemperature(float newTemp);
    float humidity() const;
    void setHumidity(float newHum);
    float pressure() const;
    void setPressure(float newPress);

private:
    float _temperature;
    float _humidity;
    float _pressure;
};


#endif // OSD_WEATHER_ENTITY_H
