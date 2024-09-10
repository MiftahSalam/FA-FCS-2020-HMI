#ifndef ENGAGEMENT_STREAM_H
#define ENGAGEMENT_STREAM_H

#include "src/infra/store/engagement/engagement_repository.h"
#include "src/shared/config/messaging_tcp_config.h"
#include "src/usecase/engagement/stream/engagement_data_40mm_stream.h"

#include <QObject>

class EngagementStream : public QObject
{
public:
    EngagementStream(
        QObject *parent = nullptr,
        MessagingTcpConfig *config = nullptr,
        EngagementRepository *repoEngage = nullptr
        );

    EngagementData40mmStream *getServiceEngagement40mm() const;

private:
    MessagingTcpConfig *consumerConfig;
    EngagementData40mmStream *serviceEngagement40mm;
};

#endif // ENGAGEMENT_STREAM_H
