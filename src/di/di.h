#ifndef DI_H
#define DI_H

#include "src/domain/weapon_assign/repository/weapon_assignment_repository.h"
#include "src/infra/store/fire_triangle/fire_triangle_repository.h"
#include "src/infra/store/gun/gun_repository.h"
#include "src/infra/store/osd/osd_repository.h"
#include "src/infra/store/track/track_repository.h"
#include "src/shared/config/configuration.h"
#include "src/usecase/gun/gun_firing_service.h"
#include "src/usecase/engagement/weapon_track_assign_service.h"
#include "src/usecase/gun/gun_manager_service.h"
#include "src/usecase/osd/cms/osd_cms.h"
#include "src/usecase/osd/stream/osd_stream.h"
#include "src/usecase/track/stream/track_stream.h"
#include "src/usecase/gun/stream/gun_stream.h"
#include "src/usecase/fire_triangle/stream/fire_triangle_stream.h"
#include "src/usecase/weapon_assign/weapon_assign_service.h"

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
    TrackRepository *getRepoTrack() const;
    FireTriangleRepository *getRepoFireTriangle() const;
    WeaponTrackAssignmentRepository *getRepoTrackWeaponAssign() const;

    TrackStream *getServiceTrackStream() const;
    GunStream *getServiceGunStream() const;
    FireTriangleStream *getServiceFireTriangle() const;

    GunManagerService *getServiceGunManager() const;
    WeaponAssignService *getServiceWeaponAssign() const;
    WeaponTrackAssignService *getServiceWeaponTrackAssign() const;
    GunFiringService *getServiceWeaponFiring() const;

protected:
    DI();

private:
    static DI *di;

    Configuration *config;

    OSDRepository *repoOSD;
    GunRepository *repoGun;
    TrackRepository *repoTrack;
    FireTriangleRepository *repoFireTriangle;
    WeaponAssignmentRepository *repoWeaponAssign;
    WeaponTrackAssignmentRepository *repoTrackWeaponAssign;

    // TODO: add weapon track engagement repository

    // TODO: add engagement correction repository

    OSDCMS *serviceOSDCMS;
    GunManagerService *serviceGunManager;
    WeaponAssignService *serviceWeaponAssign;
    GunFiringService *serviceWeaponFiring;
    WeaponTrackAssignService *serviceWeaponTrackAssign;

    // TODO: add engagement correction service

    TrackStream *serviceTrackStream;
    OSDStream *serviceOSDStream;
    GunStream *serviceGunStream;
    // TODO: add weapon track engagement stream service
    FireTriangleStream *serviceFireTriangle;
};

#endif // DI_H
