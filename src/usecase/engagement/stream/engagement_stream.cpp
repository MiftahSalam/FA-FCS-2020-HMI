#include "engagement_stream.h"
#include "src/shared/common/errors/err_object_creation.h"

EngagementStream::EngagementStream(QObject *parent, MessagingTcpConfig *config, EngagementRepository *repoEngage)
    : QObject(parent), consumerConfig(config)
{
    if (config == nullptr)
    {
        throw ErrObjectCreation();
    }

    if(repoEngage == nullptr)
    {
        throw ErrObjectCreation();
    }

    TcpMessagingOpts *engageData40mmStreamVal = config->getInstance("")->getContent().value("engagement_data");
    serviceEngagement40mm = EngagementData40mmStream::getInstance(
        engageData40mmStreamVal,
        repoEngage->getRepoEngageData()
        );
}

EngagementData40mmStream *EngagementStream::getServiceEngagement40mm() const
{
    return serviceEngagement40mm;
}
