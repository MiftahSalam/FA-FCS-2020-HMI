#ifndef DI_H
#define DI_H

#include "src/infra/store/osd/osd_repository.h"
#include "src/shared/config/configuration.h"
#include "src/usecase/osd/cms/osd_cms.h"
#include "src/usecase/osd/stream/osd_stream.h"

class DI
{
public:
    DI(DI &other) = delete;
    void operator=(const DI &) = delete;
    static DI *getInstance();

    Configuration *getConfig() const;
    OSDCMS *getOSDCMSService() const;
    OSDStream *getServiceOSDStream() const;
    OSDRepository *getRepoOSD() const;

protected:
    DI();

private:
    static DI *di;

    Configuration *config;
    OSDRepository *repoOSD;
    // TODO: add track repository
    OSDCMS *serviceOSDCMS;
    OSDStream *serviceOSDStream;
};

#endif // DI_H
