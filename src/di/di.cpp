#include "di.h"

DI* DI::di = nullptr;

DI::DI()
{
    config = new Configuration();
    repoOSD = new OSDRepository(nullptr);
    serviceOSDCMS = new OSDCMS(nullptr, config->getOsdCmsConfig(), repoOSD);
    serviceOSDStream = new OSDStream(nullptr, config->getTcpMessageConfig(), repoOSD, serviceOSDCMS);
    //    serviceOSDStream = new OSDStream(nullptr, config->getAmqpConfig());
}

OSDRepository *DI::getRepoOSD() const
{
    return repoOSD;
}

OSDStream *DI::getServiceOSDStream() const
{
    return serviceOSDStream;
}

DI *DI::getInstance()
{
    if (di == nullptr) {
        di = new DI();
    }

    return di;
}

Configuration *DI::getConfig() const
{
    return config;
}

OSDCMS *DI::getOSDCMSService() const
{
    return serviceOSDCMS;
}
