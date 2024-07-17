#ifndef OSD_WEATHER_REPOSITORY_INMEM_IMPL_H
#define OSD_WEATHER_REPOSITORY_INMEM_IMPL_H

#include "src/domain/osd/entity/osd_weather_entity.h"
#include "src/domain/osd/repository/osd_weather_repository.h"

class OSDWeatherRepositoryInMemImpl: public OSDWeatherRepository
{
public:
    static OSDWeatherRepository *GetInstance();

    // OSDBaseRepository interface
public:
    void SetSource(const std::string &source) override;
    void SetStatus(const std::string &status) override;
    void SetMode(const OSD_MODE &mode) override;

    // OSDInertiaRepository interface
public:
    void SetWeather(const OSDWeatherEntity &weather) override;
    const OSDWeatherEntity *GetWeather() const override;

protected:
    OSDWeatherRepositoryInMemImpl(OSDWeatherEntity *entity);

private:
    static OSDWeatherRepositoryInMemImpl *instance;

    OSDWeatherEntity *_entity;
};

#endif // OSD_WEATHER_REPOSITORY_INMEM_IMPL_H
