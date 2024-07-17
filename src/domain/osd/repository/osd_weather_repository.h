#ifndef OSD_WEATHER_REPOSITORY_H
#define OSD_WEATHER_REPOSITORY_H
#include "osd_base_repository.h"
#include "src/domain/osd/entity/osd_weather_entity.h"

class OSDWeatherRepository: public OSDBaseRepository
{
public:
    virtual void SetWeather(const OSDWeatherEntity &weather) = 0;
    virtual const OSDWeatherEntity* GetWeather() const = 0;
};
#endif // OSD_WEATHER_REPOSITORY_H
