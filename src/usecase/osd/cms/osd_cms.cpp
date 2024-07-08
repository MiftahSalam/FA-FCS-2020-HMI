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
    serviceOSDCMSSpeed = OSDCMSSpeedData::getInstance(
                new HttpClientWrapper(),
                cmsConfig,
                repoOSD->getRepoOSDSpeed());
}

OSDCMSPositionData *OSDCMS::getServiceOSDCMSPosition() const
{
    return serviceOSDCMSPosition;
}

OSDCMSGyroData *OSDCMS::getServiceOSDCMSGyro() const
{
    return serviceOSDCMSGyro;
}

OSDCMSSpeedData *OSDCMS::getServiceOSDCMSSpeed() const
{
    return serviceOSDCMSSpeed;
}

OSDCMSInputMode *OSDCMS::getServiceOSDCMSMode() const
{
    return serviceOSDCMSMode;
}
