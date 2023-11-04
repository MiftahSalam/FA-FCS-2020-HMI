#ifndef DI_H
#define DI_H


#include "src/shared/config/configuration.h"
#include "src/usecase/osd/cms/osd_cms.h"

class DI
{
public:
    DI();

    Configuration *getConfig() const;

private:
    Configuration *config;
    OSDCMS *serviceOSDCMS;
};

#endif // DI_H
