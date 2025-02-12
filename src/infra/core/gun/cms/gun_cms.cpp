#include "gun_cms.h"
#include "src/shared/common/errors/err_object_creation.h"

GunCMS::GunCMS(QObject *parent, GunCmsConfig *cmsConfig)
    : QObject{parent}
{
    if (cmsConfig == nullptr)
    {
        throw ErrObjectCreation();
    }

    gunCMSMode = GunCmsCommandBarrelMode::getInstance(
        new HttpClientWrapper(),
        cmsConfig);
}

GunCmsCommandBarrelMode *GunCMS::getGunCMSMode() const
{
    return gunCMSMode;
}
