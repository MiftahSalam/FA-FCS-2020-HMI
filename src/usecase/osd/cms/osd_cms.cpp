#include <QNetworkRequest>
#include <QtDebug>

#include "src/infra/http/http_client_wrapper.h"
#include "src/infra/store/osd/inmemory/osd_position_repository_inmem_impl.h"
#include "src/shared/common/errors/err_object_creation.h"
#include "osd_cms.h"


OSDCMS::OSDCMS(QObject *parent, OSDCmsConfig *cmsConfig): QObject(parent), cfgCms(cmsConfig)
{
    if(cmsConfig == nullptr) {
        throw ErrObjectCreation();
    }

    serviceOSDCMSPosition = OSDCMSPositionData::getInstance(new HttpClientWrapper(), cmsConfig);
    serviceOSDCMSMode = OSDCMSInputMode::getInstance(new HttpClientWrapper(), cmsConfig);
    repoOSDPosition = static_cast<OSDBaseRepository*>(OSDPositionRepositoryInMemImpl::GetInstance());
}


OSDCMSPositionData *OSDCMS::getServiceOSDCMSPosition() const
{
    return serviceOSDCMSPosition;
}

OSDCMSInputMode *OSDCMS::getServiceOSDCMSMode() const
{
    return serviceOSDCMSMode;
}

OSDBaseRepository *OSDCMS::getRepoOSDPosition() const
{
    return repoOSDPosition;
}
