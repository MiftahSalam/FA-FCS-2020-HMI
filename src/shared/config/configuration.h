#ifndef CONFIGURATION_H
#define CONFIGURATION_H


#include "src/shared/config/osd_cms_config.h"

class Configuration
{
public:
    Configuration();

    OSDCmsConfig *getOsdCmsConfig() const;

private:
    OSDCmsConfig *osdCmsConfig;
};

#endif // CONFIGURATION_H
