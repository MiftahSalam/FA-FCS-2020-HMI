#ifndef OSDCMS_H
#define OSDCMS_H

#include <QNetworkReply>
#include <QObject>

#include "src/shared/config/osd_cms_config.h"
#include "src/usecase/osd/cms/osd_cms_input_mode.h"
#include "src/usecase/osd/cms/osd_cms_position_data.h"
#include "src/usecase/osd/cms/osd_cms_speed_data.h"

class OSDCMS : public QObject
{
    Q_OBJECT
public:
    OSDCMS(QObject *parent = nullptr, OSDCmsConfig *cmsConfig = nullptr);

    OSDCMSPositionData *getServiceOSDCMSPosition() const;
    OSDCMSSpeedData *getServiceOSDCMSSpeed() const;
    OSDCMSInputMode *getServiceOSDCMSMode() const;

private:
    OSDCmsConfig *cfgCms;
    OSDCMSPositionData *serviceOSDCMSPosition;
    OSDCMSSpeedData *serviceOSDCMSSpeed;

    OSDCMSInputMode *serviceOSDCMSMode;
};

#endif // OSDCMS_H
