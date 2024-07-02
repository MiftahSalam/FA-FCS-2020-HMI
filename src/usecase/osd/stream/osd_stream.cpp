#include "osd_stream.h"
#include "src/shared/common/errors/err_object_creation.h"

// OSDStream::OSDStream(QObject *parent, AMQPConfig *config): QObject(parent), cfgAmqp(config)
//{
//     if(cfgAmqp == nullptr) {
//         throw ErrObjectCreation();
//     }

//    serviceOSDStreamPosition = OSDStreamPosition::getInstance(cfgAmqp);
//}

OSDStream::OSDStream(QObject *parent, MessagingTcpConfig *config, OSDRepository *repoOSD) : QObject(parent), consumerConfig(config)
{
    if (config == nullptr)
    {
        throw ErrObjectCreation();
    }

    if (repoOSD == nullptr)
    {
        throw ErrObjectCreation();
    }

    TcpMessagingOpts *posStreamVal = config->getInstance("")->getContent().value("position");
    serviceOSDStreamPosition = OSDStreamPosition::getInstance(posStreamVal, repoOSD->getRepoOSDPosition());

    TcpMessagingOpts *gyroStreamVal = config->getInstance("")->getContent().value("inertia");
    serviceOSDStreamGyro = OSDStreamGyro::getInstance(gyroStreamVal);
}

OSDStreamPosition *OSDStream::getServiceOSDStreamPosition() const
{
    return serviceOSDStreamPosition;
}

OSDStreamGyro *OSDStream::getServiceOSDStreamGyro() const
{
    return serviceOSDStreamGyro;
}
