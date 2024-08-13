#ifndef GUN_STREAM_H
#define GUN_STREAM_H

#include "src/infra/store/gun/gun_repository.h"
#include "src/shared/config/messaging_tcp_config.h"
#include "src/usecase/gun/stream/gun_coverage_stream.h"

#include <QObject>

class GunStream : public QObject
{
public:
    GunStream(
        QObject *parent = nullptr,
        MessagingTcpConfig *config = nullptr,
        GunRepository *repoGun = nullptr
        );

    GunCoverageStream *getServiceGunCoverage() const;

private:
    MessagingTcpConfig *consumerConfig;
    GunCoverageStream *serviceGunCoverage;

};

#endif // GUN_STREAM_H
