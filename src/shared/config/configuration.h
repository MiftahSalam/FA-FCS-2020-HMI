#ifndef CONFIGURATION_H
#define CONFIGURATION_H


#include "src/shared/config/amqp_config.h"
#include "src/shared/config/messaging_tcp_config.h"
#include "src/shared/config/osd_cms_config.h"

class Configuration
{
public:
    Configuration();

    OSDCmsConfig *getOsdCmsConfig() const;

    AMQPConfig *getAmqpConfig() const;

    MessagingTcpConfig *getTcpMessageConfig() const;

private:
    OSDCmsConfig *osdCmsConfig;
//    AMQPConfig *amqpConfig;
    MessagingTcpConfig *tcpMessageConfig;
};

#endif // CONFIGURATION_H
