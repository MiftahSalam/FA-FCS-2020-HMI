#include "osd_weather_repository_inmem_impl.h"

OSDWeatherRepositoryInMemImpl* OSDWeatherRepositoryInMemImpl::instance{nullptr};

OSDWeatherRepositoryInMemImpl::OSDWeatherRepositoryInMemImpl(OSDWeatherEntity *entity)
{
    _entity = entity;
}

OSDWeatherRepository *OSDWeatherRepositoryInMemImpl::GetInstance()
{
    if (instance == nullptr) {
        OSDWeatherEntity* entity = new OSDWeatherEntity(0, 0, 0, "", "", OSD_MODE::AUTO);
        instance = new OSDWeatherRepositoryInMemImpl(entity);
    }
    return instance;
}

void OSDWeatherRepositoryInMemImpl::SetSource(const std::string &source)
{
    _entity->setSource(source);
}

void OSDWeatherRepositoryInMemImpl::SetStatus(const std::string &status)
{
    _entity->setStatus(status);
}

void OSDWeatherRepositoryInMemImpl::SetMode(const OSD_MODE &mode)
{
    _entity->setMode(mode);
}

void OSDWeatherRepositoryInMemImpl::SetWeather(const OSDWeatherEntity &weather)
{
    _entity->setTemp(weather.temp());
    _entity->setHum(weather.hum());
    _entity->setPress(weather.press());
}

const OSDWeatherEntity *OSDWeatherRepositoryInMemImpl::GetWeather() const
{
    return _entity;
}
