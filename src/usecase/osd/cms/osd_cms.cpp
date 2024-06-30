#include <QNetworkRequest>
#include <QtDebug>

#include "src/infra/http/http_client_wrapper.h"
#include "src/shared/common/errors/err_object_creation.h"
#include "osd_cms.h"


OSDCMS::OSDCMS(QObject *parent, OSDCmsConfig *cmsConfig): QObject(parent), cfgCms(cmsConfig)
{
    if(cmsConfig == nullptr) {
        throw ErrObjectCreation();
    }

    serviceOSDCMSPosition = OSDCMSPositionData::getInstance(new HttpClientWrapper(), cmsConfig);
    serviceOSDCMSSpeed = OSDCMSSpeedData::getInstance(new HttpClientWrapper(), cmsConfig);
    serviceOSDCMSMode = OSDCMSInputMode::getInstance(new HttpClientWrapper(), cmsConfig);
}


OSDCMSPositionData *OSDCMS::getServiceOSDCMSPosition() const
{
    return serviceOSDCMSPosition;
}

OSDCMSSpeedData *OSDCMS::getServiceOSDCMSSpeed() const
{
    return serviceOSDCMSSpeed;
}

OSDCMSInputMode *OSDCMS::getServiceOSDCMSMode() const
{
    return serviceOSDCMSMode;
}
