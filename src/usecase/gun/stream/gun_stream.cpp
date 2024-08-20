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
    serviceGunCoverage = GunCoverageStream::getInstance(
        gunCovStreamVal,
        repoGun->getRepoGunCoverage()
        );

    TcpMessagingOpts *gunFbackStreamVal = config->getInstance("")->getContent().value("gun_feedback");
    serviceGunFBack = GunFeedbackStream::getInstance(
        gunFbackStreamVal,
        repoGun->getRepoGunFeedback()
        );
}

GunCoverageStream *GunStream::getServiceGunCoverage() const
{
    return serviceGunCoverage;
}

GunFeedbackStream *GunStream::getServiceGunFeedback() const
{
    return serviceGunFBack;
}
