#ifndef OSDREPOSITORY_H
#define OSDREPOSITORY_H

#include "src/domain/osd/repository/osd_inertia_repository.h"
#include "src/domain/osd/repository/osd_position_repository.h"
#include "src/domain/osd/repository/osd_waterspeed_repository.h"
#include "src/domain/osd/repository/osd_speed_repository.h"
#include "src/domain/osd/repository/osd_wind_repository.h"
#include "src/domain/osd/repository/osd_weather_repository.h"

#include <QObject>

class OSDRepository : public QObject
{
    Q_OBJECT
public:
    explicit OSDRepository(QObject *parent = nullptr);

    OSDPositionRepository *getRepoOSDPosition() const;
    OSDInertiaRepository *getRepoOSDInertia() const;
    OSDWaterSpeedRepository *getRepoOSDWaterSpeed() const;
    OSDSpeedRepository *getRepoOSDSpeed() const;
    OSDWindRepository *getRepoOSDWind() const;
    OSDWeatherRepository *getRepoOSDWeather() const;

private:
    OSDPositionRepository *repoOSDPosition;
    OSDInertiaRepository *repoOSDInertia;
    OSDWaterSpeedRepository *repoOSDWaterSpeed;
    OSDSpeedRepository *repoOSDSpeed;
    OSDWindRepository *repoOSDWind;
    OSDWeatherRepository *repoOSDWeather;
};

#endif // OSDREPOSITORY_H
