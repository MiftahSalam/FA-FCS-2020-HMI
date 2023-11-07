#include "di.h"

DI* DI::di = nullptr;

DI::DI()
{
    config = new Configuration();
    serviceOSDCMS = new OSDCMS(nullptr, config->getOsdCmsConfig());
    serviceOSDStream = new OSDStream(nullptr, config->getTcpMessageConfig());
//    serviceOSDStream = new OSDStream(nullptr, config->getAmqpConfig());
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
