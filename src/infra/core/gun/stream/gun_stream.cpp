#include "gun_stream.h"
#include "src/shared/common/errors/err_object_creation.h"

GunStream::GunStream(
    QObject *parent,
    MessagingTcpConfig *config,
    GunRepository *repoGun
    ): QObject(parent), consumerConfig(config)
{
    if (config == nullptr)
    {
        throw ErrObjectCreation();
    }

    if (repoGun == nullptr)
    {
        throw ErrObjectCreation();
    }

    // read config file
    TcpMessagingOpts *gunCovStreamVal = config->getInstance("")->getContent().value("gun_coverage");
    streamGunCoverage = GunCoverageStream::getInstance(
        gunCovStreamVal
        );

    TcpMessagingOpts *gunFbackStreamVal = config->getInstance("")->getContent().value("gun_status_feedback");
    streamGunStatus = GunFeedbackStatusStream::getInstance(
        gunFbackStreamVal
        );

    TcpMessagingOpts *gunBarrelStreamVal = config->getInstance("")->getContent().value("gun_barrel_feedback");
    streamGunBarrel = GunFeedbackBarrelStream::getInstance(
        gunBarrelStreamVal
        );
}

GunCoverageStream *GunStream::getStreamGunCoverage() const
{
    return streamGunCoverage;
}

GunFeedbackStatusStream *GunStream::getStreamGunStatus() const
{
    return streamGunStatus;
}

GunFeedbackBarrelStream *GunStream::getStreamGunBarrel() const
{
    return streamGunBarrel;
}
