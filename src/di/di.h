#ifndef DI_H
#define DI_H

#include "src/infra/store/gun/gun_repository.h"
#include "src/infra/store/osd/osd_repository.h"
#include "src/shared/config/configuration.h"
#include "src/usecase/osd/cms/osd_cms.h"
#include "src/usecase/osd/stream/osd_stream.h"
#include "src/usecase/track/stream/track_stream.h"
#include "src/infra/store/track/track_repository.h"

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
    TrackStream *getServiceTrackStream() const;
    TrackRepository *getRepoTrack() const;

protected:
    DI();

private:
    static DI *di;

    Configuration *config;

    OSDRepository *repoOSD;
    GunRepository *repoGun;
    // TODO: add track repository
    TrackRepository *repoTrack;
    // TODO: add weapon assignment repository

    // TODO: add weapon track engagement repository

    // TODO: add engagement correction repository

    OSDCMS *serviceOSDCMS;
    // TODO: add gun command service

    // TODO: add weapon assignment service

    // TODO: add weapon track engagement service
    TrackStream *serviceTrackStream;
    // TODO: add engagement correction service

    OSDStream *serviceOSDStream;
    // TODO: add gun stream service

    // TODO: add weapon track engagement stream service

};

#endif // DI_H
