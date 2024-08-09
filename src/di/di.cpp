#include "di.h"

DI* DI::di = nullptr;

DI::DI()
{
    config = new Configuration();

    repoOSD = new OSDRepository(nullptr);
    repoGun = new GunRepository(nullptr);
    repoTrack = new TrackRepository(nullptr);

    // TODO: add weapon assignment repository

    // TODO: add weapon track engagement repository
    // TODO: add engagement correction repository

    serviceOSDCMS = new OSDCMS(nullptr, config->getOsdCmsConfig(), repoOSD);
    // TODO: add gun command service

    // TODO: add weapon assignment service

    // TODO: add weapon track engagement service
    serviceTrackStream = new TrackStream(nullptr, config->getTcpMessageConfig(), repoTrack);
    // TODO: add engagement correction service

    serviceOSDStream = new OSDStream(nullptr, config->getTcpMessageConfig(), repoOSD, serviceOSDCMS);
    // TODO: add gun stream service

    // TODO: add weapon track engagement stream service

    //    serviceOSDStream = new OSDStream(nullptr, config->getAmqpConfig());

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
