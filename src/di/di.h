#ifndef DI_H
#define DI_H

#include "src/domain/weapon_assign/repository/weapon_assignment_repository.h"
#include "src/infra/core/gun/cms/gun_cms.h"
#include "src/infra/core/osd/cms/osd_cms.h"
#include "src/infra/store/fire_triangle/fire_triangle_repository.h"
#include "src/infra/store/gun/gun_repository.h"
#include "src/infra/store/osd/osd_repository.h"
#include "src/infra/store/track/track_repository.h"
#include "src/shared/config/configuration.h"
#include "src/usecase/gun/gun_firing_service.h"
#include "src/usecase/engagement/weapon_track_assign_service.h"
#include "src/usecase/gun/gun_manager_service.h"
#include "src/infra/core/osd/stream/osd_stream.h"
#include "src/usecase/osd/osd_service.h"
#include "src/usecase/track/stream/track_stream.h"
#include "src/infra/core/gun/stream/gun_stream.h"
#include "src/usecase/fire_triangle/stream/fire_triangle_stream.h"
#include "src/usecase/weapon_assign/weapon_assign_service.h"
#include "src/infra/store/engagement/engagement_repository.h"
#include "src/usecase/engagement/stream/engagement_stream.h"

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
    EngagementRepository *getRepoEngagementData() const;

    TrackStream *getServiceTrackStream() const;
    GunStream *getServiceGunStream() const;
    FireTriangleStream *getServiceFireTriangle() const;
    EngagementStream *getServiceEngagementStream() const;

    OSDService *getServiceOSD() const;
    GunManagerService *getServiceGunManager() const;
    WeaponAssignService *getServiceWeaponAssign() const;
    WeaponTrackAssignService *getServiceWeaponTrackAssign() const;
    GunFiringService *getServiceWeaponFiring() const;
    GunBarrelControlModeService *getServiceGunMode() const;

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
    EngagementRepository *repoEngagement;

    OSDCMS *serviceOSDCMS;
    GunCMS *gunCmses;

    OSDService *serviceOSD;
    GunBarrelControlModeService *serviceGunMode;
    GunManagerService *serviceGunManager;
    WeaponAssignService *serviceWeaponAssign;
    GunFiringService *serviceWeaponFiring;
    WeaponTrackAssignService *serviceWeaponTrackAssign;

    TrackStream *serviceTrackStream;
    OSDStream *serviceOSDStream;
    GunStream *serviceGunStream;
    FireTriangleStream *serviceFireTriangle;
    EngagementStream *serviceEngagementStream;
};

#endif // DI_H
