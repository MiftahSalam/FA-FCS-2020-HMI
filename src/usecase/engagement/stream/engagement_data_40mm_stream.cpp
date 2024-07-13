#include "engagement_data_40mm_stream.h"
#include "src/shared/common/errors/err_object_creation.h"

EngagementData40mmStream* EngagementData40mmStream::instance = nullptr;

EngagementData40mmStream::EngagementData40mmStream(
        TcpMessagingOpts *config,
        EngagementDataRepository *repoEngagemenData)
    : cfg(config), _repoEngagemenData(repoEngagemenData)
{
    consumer = new TcpMessagingWrapper(this, config);
    connect(consumer, &TcpMessagingWrapper::signalForwardMessage, this, &EngagementData40mmStream::onDataReceived);
}

EngagementData40mmStream *EngagementData40mmStream::getInstance(
        TcpMessagingOpts *config = nullptr,
        EngagementDataRepository* repoEngagemenData = nullptr
        )
{
    if (instance == nullptr) {
        if(config == nullptr) {
            throw ErrObjectCreation();
        }

        if(repoEngagemenData == nullptr) {
            throw ErrObjectCreation();
        }

        instance = new EngagementData40mmStream(config, repoEngagemenData);
    }

    return instance;
}

void EngagementData40mmStream::onDataReceived(QByteArray data)
{
}

BaseError EngagementData40mmStream::check()
{
}
