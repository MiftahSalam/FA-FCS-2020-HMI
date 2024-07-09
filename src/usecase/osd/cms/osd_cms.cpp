#include <QNetworkRequest>
#include <QtDebug>

#include "src/infra/http/http_client_wrapper.h"
#include "src/shared/common/errors/err_object_creation.h"
#include "osd_cms.h"

OSDCMS::OSDCMS(QObject *parent, OSDCmsConfig *cmsConfig, OSDRepository *repoOSD)
    : QObject(parent), cfgCms(cmsConfig)
{
    if (cmsConfig == nullptr)
    {
        throw ErrObjectCreation();
    }

    if (repoOSD == nullptr)
    {
        throw ErrObjectCreation();
    }

    serviceOSDCMSMode = OSDCMSInputMode::getInstance(
                new HttpClientWrapper(),
                cmsConfig,
                repoOSD->getRepoOSDPosition());
    serviceOSDCMSPosition = OSDCMSPositionData::getInstance(
                new HttpClientWrapper(),
                cmsConfig,
                repoOSD->getRepoOSDPosition());
    serviceOSDCMSGyro = OSDCMSGyroData::getInstance(
                new HttpClientWrapper(),
                cmsConfig,
                repoOSD->getRepoOSDInertia());
    serviceOSDCMSWaterSpeed = OSDCMSWaterSpeedData::getInstance(
                new HttpClientWrapper(),
                cmsConfig,
                repoOSD->getRepoOSDWaterSpeed());
}

OSDCMSInputMode *OSDCMS::getServiceOSDCMSMode() const
{
    return serviceOSDCMSMode;
}

OSDCMSPositionData *OSDCMS::getServiceOSDCMSPosition() const
{
    return serviceOSDCMSPosition;
}

OSDCMSGyroData *OSDCMS::getServiceOSDCMSGyro() const
{
    return serviceOSDCMSGyro;
}

OSDCMSWaterSpeedData *OSDCMS::getServiceOSDCMSWaterSpeed() const
{
    return serviceOSDCMSWaterSpeed;
}
