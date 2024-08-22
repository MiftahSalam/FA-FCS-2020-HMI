#ifndef DI_H
#define DI_H

#include "src/infra/store/fire_triangle/fire_triangle_repository.h"
#include "src/infra/store/gun/gun_repository.h"
#include "src/infra/store/osd/osd_repository.h"
#include "src/infra/store/track/track_repository.h"
#include "src/shared/config/configuration.h"
#include "src/usecase/gun/gun_manager_service.h"
#include "src/usecase/osd/cms/osd_cms.h"
#include "src/usecase/osd/stream/osd_stream.h"
#include "src/usecase/track/stream/track_stream.h"
#include "src/usecase/gun/stream/gun_stream.h"

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
    FireTriangleRepository *getRepoFireTriangle() const;
    GunStream *getServiceGunStream() const;

    // GunCommandService *getGunCMSService() const;
    // GunFeedbackStream *getServiceGunStream() const;

    GunManagerService *getServiceGunManager() const;

protected:
    DI();

private:
    static DI *di;

    Configuration *config;

    OSDRepository *repoOSD;
    GunRepository *repoGun;
    TrackRepository *repoTrack;
    FireTriangleRepository *repoFireTriangle;

    // TODO: add weapon assignment repository

    // TODO: add weapon track engagement repository

    // TODO: add engagement correction repository

    OSDCMS *serviceOSDCMS;
    GunManagerService *serviceGunManager;

    // TODO: add weapon assignment service

    // TODO: add weapon track engagement service
    TrackStream *serviceTrackStream;
    // TODO: add engagement correction service

    OSDStream *serviceOSDStream;
    // TODO: add gun stream service
    GunStream *serviceGunStream;
    // TODO: add weapon track engagement stream service

};

#endif // DI_H
