#ifndef OSDCMS_H
#define OSDCMS_H

#include <QNetworkReply>
#include <QObject>

#include "src/infra/store/osd/osd_repository.h"
#include "src/shared/config/osd_cms_config.h"
#include "src/usecase/osd/cms/osd_cms_input_mode.h"
#include "src/usecase/osd/cms/osd_cms_position_data.h"
#include "src/usecase/osd/cms/osd_cms_gyro_data.h"
#include "src/usecase/osd/cms/osd_cms_waterspeed_data.h"
#include "src/usecase/osd/cms/osd_cms_speed_data.h"

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

private:
    OSDCmsConfig *cfgCms;
    OSDCMSInputMode *serviceOSDCMSMode;
    OSDCMSPositionData *serviceOSDCMSPosition;
    OSDCMSGyroData *serviceOSDCMSGyro;
    OSDCMSWaterSpeedData *serviceOSDCMSWaterSpeed;
    OSDCMSSpeedData *serviceOSDCMSSpeed;
};

#endif // OSDCMS_H
