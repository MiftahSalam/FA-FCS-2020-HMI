#include "di.h"
#include "src/infra/store/weapon_assign/inmemory/weapon_assignment_repository_inmem_impl.h"

DI* DI::di = nullptr;

DI::DI()
{
    config = new Configuration();

    repoOSD = new OSDRepository(nullptr);
    repoGun = new GunRepository(nullptr);
    repoTrack = new TrackRepository(nullptr);
    repoFireTriangle = new FireTriangleRepository(nullptr);
    repoWeaponAssign = WeaponAssignmentRepositoryInMemImpl::GetInstance();

    // TODO: add weapon track engagement repository
    // TODO: add engagement correction repository

    serviceOSDCMS = new OSDCMS(nullptr, config->getOsdCmsConfig(), repoOSD);
    serviceGunManager = GunManagerService::getInstance(nullptr, config->getGunCmsConfig(), repoGun->getRepoGunFeedback(), repoGun->getRepoGunCmd());
    serviceWeaponAssign = WeaponAssignService::getInstance(nullptr, repoWeaponAssign);
    // TODO: add weapon assignment service

    // TODO: add weapon track engagement service
    serviceTrackStream = new TrackStream(nullptr, config->getTcpMessageConfig(), config->getArpaConfig(), repoTrack);
    // TODO: add engagement correction service

    serviceOSDStream = new OSDStream(nullptr, config->getTcpMessageConfig(), repoOSD, serviceOSDCMS);
    serviceGunStream = new GunStream(nullptr, config->getTcpMessageConfig(), repoGun);
    // TODO: add weapon track engagement stream service
    serviceFireTriangle = new FireTriangleStream(nullptr, config->getTcpMessageConfig(), repoFireTriangle);
}

WeaponAssignService *DI::getServiceWeaponAssign() const
{
    return serviceWeaponAssign;
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
    if (di == nullptr) {
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
