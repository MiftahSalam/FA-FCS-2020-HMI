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
}

GunCoverageStream *GunStream::getServiceGunCoverage() const
{
    return serviceGunCoverage;
}
