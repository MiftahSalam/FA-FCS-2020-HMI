#ifndef OSDCMS_H
#define OSDCMS_H

#include <QNetworkReply>
#include <QObject>

#include "src/infra/core/cms/osd/input_mode/osd_cms_input_mode.h"
#include "src/infra/store/osd/osd_repository.h"
#include "src/infra/core/cms/osd/position/osd_cms_position_data.h"
#include "src/infra/core/cms/osd/inertia/osd_cms_gyro_data.h"
#include "src/infra/core/cms/osd/water_speed/osd_cms_waterspeed_data.h"
#include "src/infra/core/cms/osd/speed/osd_cms_speed_data.h"
#include "src/infra/core/cms/osd/wind/osd_cms_wind_data.h"
#include "src/infra/core/cms/osd/weather/osd_cms_weather_data.h"
#include "src/shared/config/osd_cms_config.h"

class OSDCMS : public QObject
{
    Q_OBJECT
public:
    OSDCMS(
            QObject *parent = nullptr,
            OSDCmsConfig *cmsConfig = nullptr,
            OSDRepository *repoOSD = nullptr
            );

    OSDCMSInputMode *getServiceOSDCMSMode() const;
    OSDCMSPositionData *getServiceOSDCMSPosition() const;
    OSDCMSGyroData *getServiceOSDCMSGyro() const;
    OSDCMSWaterSpeedData *getServiceOSDCMSWaterSpeed() const;
    OSDCMSSpeedData *getServiceOSDCMSSpeed() const;
    OSDCMSWindData *getServiceOSDCMSWind() const;
    OSDCMSWeatherData *getServiceOSDCMSWeather() const;

private:
    OSDCmsConfig *cfgCms;
    OSDCMSInputMode *serviceOSDCMSMode;
    OSDCMSPositionData *serviceOSDCMSPosition;
    OSDCMSGyroData *serviceOSDCMSGyro;
    OSDCMSWaterSpeedData *serviceOSDCMSWaterSpeed;
    OSDCMSSpeedData *serviceOSDCMSSpeed;
    OSDCMSWindData *serviceOSDCMSWind;
    OSDCMSWeatherData *serviceOSDCMSWeather;
};

#endif // OSDCMS_H
