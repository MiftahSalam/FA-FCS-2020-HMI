#include "di.h"

DI* DI::di = nullptr;

DI::DI()
{
    config = new Configuration();
    serviceOSDCMS = new OSDCMS(nullptr, config->getOsdCmsConfig());
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
