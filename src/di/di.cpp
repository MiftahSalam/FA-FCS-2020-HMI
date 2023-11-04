#include "di.h"

DI::DI()
{
    config = new Configuration();
    serviceOSDCMS = new OSDCMS(nullptr, config->getOsdCmsConfig());
}

Configuration *DI::getConfig() const
{
    return config;
}
