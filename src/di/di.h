#ifndef DI_H
#define DI_H


#include "src/shared/config/configuration.h"
#include "src/usecase/osd/cms/osd_cms.h"

class DI
{
public:
    DI(DI &other) = delete;
    void operator=(const DI&) = delete;
    static DI* getInstance();

    Configuration *getConfig() const;
    OSDCMS *getOSDCMSService() const;

protected:
    DI();

private:
    static DI *di;

    Configuration *config;
    OSDCMS *serviceOSDCMS;
};

#endif // DI_H
