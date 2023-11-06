#include "osd_stream.h"
#include "src/shared/common/errors/err_object_creation.h"

OSDStream::OSDStream(QObject *parent, AMQPConfig *config): QObject(parent), cfgAmqp(config)
{
    if(cfgAmqp == nullptr) {
        throw ErrObjectCreation();
    }

    serviceOSDStreamPosition = OSDStreamPosition::getInstance(cfgAmqp);
}

OSDStreamPosition *OSDStream::getServiceOSDStreamPosition() const
{
    return serviceOSDStreamPosition;
}
