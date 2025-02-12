#ifndef GUN_STREAM_H
#define GUN_STREAM_H

#include "src/infra/store/gun/gun_repository.h"
#include "src/shared/config/messaging_tcp_config.h"
#include "src/infra/core/gun/stream/gun_coverage/gun_coverage_stream.h"
#include "src/infra/core/gun/stream/gun_status/gun_feedback_status_stream.h"
#include "src/infra/core/gun/stream/gun_barrel/gun_feedback_barrel_stream.h"

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
    GunFeedbackStatusStream *getServiceGunFeedback() const;
    GunFeedbackBarrelStream *getServiceGunBarrel() const;

private:
    MessagingTcpConfig *consumerConfig;
    GunCoverageStream *serviceGunCoverage;
    GunFeedbackStatusStream *serviceGunFBack;
    GunFeedbackBarrelStream *serviceGunBarrel;

};

#endif // GUN_STREAM_H
