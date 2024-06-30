#include <QNetworkRequest>
#include <QtDebug>

#include "src/infra/http/http_client_wrapper.h"
#include "src/shared/common/errors/err_object_creation.h"
#include "osd_cms.h"


OSDCMS::OSDCMS(QObject *parent, OSDCmsConfig *cmsConfig, OSDRepository *repoOSD): QObject(parent), cfgCms(cmsConfig)
{
    if(cmsConfig == nullptr) {
        throw ErrObjectCreation();
    }

    if(repoOSD == nullptr) {
        throw ErrObjectCreation();
    }

    serviceOSDCMSPosition = OSDCMSPositionData::getInstance(new HttpClientWrapper(), cmsConfig, repoOSD->getRepoOSDPosition());
    serviceOSDCMSMode = OSDCMSInputMode::getInstance(new HttpClientWrapper(), cmsConfig, repoOSD->getRepoOSDPosition());
}


OSDCMSPositionData *OSDCMS::getServiceOSDCMSPosition() const
{
    return serviceOSDCMSPosition;
}

OSDCMSInputMode *OSDCMS::getServiceOSDCMSMode() const
{
    return serviceOSDCMSMode;
}
