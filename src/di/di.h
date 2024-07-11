#ifndef DI_H
#define DI_H

#include "src/infra/store/gun/gun_repository.h"
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

    GunRepository *getRepoGun() const;

protected:
    DI();

private:
    static DI *di;

    Configuration *config;

    OSDRepository *repoOSD;
    GunRepository *repoGun;
    // TODO: add track repository

    OSDCMS *serviceOSDCMS;
    // TODO: add gun command service

    OSDStream *serviceOSDStream;
    // TODO: add gun stream service
};

#endif // DI_H
