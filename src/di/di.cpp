#include "di.h"
#include "src/infra/store/weapon_assign/inmemory/weapon_assignment_repository_inmem_impl.h"
#include "src/infra/store/weapon_track_assign/weapon_track_assignment_repository_inmem_impl.h"

DI *DI::di = nullptr;

DI::DI()
{
    config = new Configuration();

    repoOSD = new OSDRepository(nullptr);
    repoGun = new GunRepository(nullptr);
    repoTrack = new TrackRepository(nullptr);
    repoFireTriangle = new FireTriangleRepository(nullptr);
    repoWeaponAssign = WeaponAssignmentRepositoryInMemImpl::GetInstance();
    repoTrackWeaponAssign = WeaponTrackAssignmentRepositoryInMemImpl::GetInstance();
    repoEngagement = new EngagementRepository(nullptr);

    serviceOSDCMS = new OSDCMS(nullptr, config->getOsdCmsConfig());
    serviceOSDStream = new OSDStream(nullptr, config->getTcpMessageConfig());
    serviceOSD = OSDService::getInstance(nullptr, repoOSD, serviceOSDCMS, serviceOSDStream);
    gunCmses = new GunCMS(nullptr, config->getGunCmsConfig());
    serviceGunMode = GunBarrelControlModeService::getInstance(
        nullptr,
        repoGun->getRepoGunCmd(),
        gunCmses->getGunCMSMode());
    serviceGunManager = GunManagerService::getInstance(
        nullptr,
        config->getGunCmsConfig(),
        repoGun->getRepoGunFeedback(),
        repoGun->getRepoGunCmd(),
        serviceGunMode
        );
    serviceWeaponAssign = WeaponAssignService::getInstance(nullptr, repoWeaponAssign);
    serviceWeaponTrackAssign = WeaponTrackAssignService::getInstance(
                nullptr,
                config->getTrackWeaponAssignCmsConfig(),
                repoGun->getRepoGunCoverage(),
                repoTrack->getRepoTrackArpa(),
                serviceOSD,
                repoWeaponAssign,
                repoTrackWeaponAssign);
    //    serviceWeaponFiring = GunFiringService::getInstance(nullptr, config->getSerialMessageConfig(), repoGun->getRepoGunFeedback(), serviceWeaponAssign, serviceWeaponTrackAssign);
    serviceWeaponFiring = GunFiringService::getInstance(
                nullptr,
                config->getTcpMessageConfig(),
                serviceGunManager,
                serviceWeaponAssign,
                serviceWeaponTrackAssign);

    serviceTrackStream = new TrackStream(nullptr, config->getTcpMessageConfig(), config->getArpaConfig(), repoTrack);

    serviceGunStream = new GunStream(nullptr, config->getTcpMessageConfig(), repoGun);
    serviceFireTriangle = new FireTriangleStream(nullptr, config->getTcpMessageConfig(), repoFireTriangle);
    serviceEngagementStream = new EngagementStream(nullptr, config->getTcpMessageConfig(), repoEngagement);
}

GunBarrelControlModeService *DI::getServiceGunMode() const
{
    return serviceGunMode;
}

OSDService *DI::getServiceOSD() const
{
    return serviceOSD;
}

GunFiringService *DI::getServiceWeaponFiring() const
{
    return serviceWeaponFiring;
}

WeaponTrackAssignmentRepository *DI::getRepoTrackWeaponAssign() const
{
    return repoTrackWeaponAssign;
}

WeaponTrackAssignService *DI::getServiceWeaponTrackAssign() const
{
    return serviceWeaponTrackAssign;
}

WeaponAssignService *DI::getServiceWeaponAssign() const
{
    return serviceWeaponAssign;
}

EngagementStream *DI::getServiceEngagementStream() const
{
    return serviceEngagementStream;
}

GunManagerService *DI::getServiceGunManager() const
{
    return serviceGunManager;
}

GunRepository *DI::getRepoGun() const
{
    return repoGun;
}

TrackStream *DI::getServiceTrackStream() const
{
    return serviceTrackStream;
}

TrackRepository *DI::getRepoTrack() const
{
    return repoTrack;
}

FireTriangleRepository *DI::getRepoFireTriangle() const
{
    return repoFireTriangle;
}

GunStream *DI::getServiceGunStream() const
{
    return serviceGunStream;
}

FireTriangleStream *DI::getServiceFireTriangle() const
{
    return serviceFireTriangle;
}

OSDRepository *DI::getRepoOSD() const
{
    return repoOSD;
}

OSDStream *DI::getServiceOSDStream() const
{
    return serviceOSDStream;
}

DI *DI::getInstance()
{
    if (di == nullptr)
    {
        di = new DI();
    }

    return di;
}

Configuration *DI::getConfig() const
{
    return config;
}

OSDCMS *DI::getOSDCMSService() const
{
    return serviceOSDCMS;
}
