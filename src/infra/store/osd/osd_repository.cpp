#include "osd_repository.h"
#include "src/infra/store/osd/inmemory/osd_input_mode_repository_inmem_impl.h"
#include "src/infra/store/osd/inmemory/osd_position_repository_inmem_impl.h"
#include "src/infra/store/osd/inmemory/osd_inertia_repository_inmem_impl.h"
#include "src/infra/store/osd/inmemory/osd_waterspeed_repository_inmem_impl.h"
#include "src/infra/store/osd/inmemory/osd_speed_repository_inmem_impl.h"
#include "src/infra/store/osd/inmemory/osd_wind_repository_inmem_impl.h"
#include "src/infra/store/osd/inmemory/osd_weather_repository_inmem_impl.h"
#include "src/infra/store/osd/inmemory/osd_datetime_repository_inmem_impl.h"


OSDRepository::OSDRepository(QObject *parent)
    : QObject{parent}
{
    repoOSDMode = OSDInputModeRepositoryInMemImpl::GetInstance();
    repoOSDPosition = OSDPositionRepositoryInMemImpl::GetInstance();
    repoOSDInertia = OSDInertiaRepositoryInMemImpl::GetInstance();
    repoOSDWaterSpeed = OSDWaterSpeedRepositoryInMemImpl::GetInstance();
    repoOSDSpeed = OSDSpeedRepositoryInMemImpl::GetInstance();
    repoOSDWind = OSDWindRepositoryInMemImpl::GetInstance();
    repoOSDWeather = OSDWeatherRepositoryInMemImpl::GetInstance();
    repoDateTime = OSDDateTimeRepositoryInMemImpl::GetInstance();
}

OSDPositionRepository *OSDRepository::getRepoOSDPosition() const
{
    return repoOSDPosition;
}

OSDInertiaRepository *OSDRepository::getRepoOSDInertia() const
{
    return repoOSDInertia;
}

OSDWaterSpeedRepository *OSDRepository::getRepoOSDWaterSpeed() const
{
    return repoOSDWaterSpeed;
}

OSDSpeedRepository *OSDRepository::getRepoOSDSpeed() const
{
    return repoOSDSpeed;
}

OSDWindRepository *OSDRepository::getRepoOSDWind() const
{
    return repoOSDWind;
}

OSDWeatherRepository *OSDRepository::getRepoOSDWeather() const
{
    return repoOSDWeather;
}

OSDDateTimeRepository *OSDRepository::getRepoDateTime() const
{
    return repoDateTime;
}

OSDInputModeRepository *OSDRepository::getRepoOSDMode() const
{
    return repoOSDMode;
}
