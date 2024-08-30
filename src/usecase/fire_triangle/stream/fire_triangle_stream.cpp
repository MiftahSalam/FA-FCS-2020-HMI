#include "fire_triangle_stream.h"
#include "src/shared/common/errors/err_object_creation.h"

FireTriangleStream::FireTriangleStream(
    QObject *parent,
    MessagingTcpConfig *config,
    FireTriangleRepository *repoFta
    ): QObject(parent), consumerConfig(config)
{
    if (config == nullptr)
    {
        throw ErrObjectCreation();
    }

    if (repoFta == nullptr)
    {
        throw ErrObjectCreation();

    }

    // read config file
    TcpMessagingOpts *fireTAStreamVal = config->getInstance("")->getContent().value("fire_triangle");
    serviceFireTriangle = FireTriangleService::getInstance(
        fireTAStreamVal,
        repoFta->getRepoFT40mm()
        );
}

FireTriangleService *FireTriangleStream::getServiceFireTriangle() const
{
    return serviceFireTriangle;
}
