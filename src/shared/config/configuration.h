#ifndef CONFIGURATION_H
#define CONFIGURATION_H


#include "src/shared/config/amqp_config.h"
#include "src/shared/config/arpa_config.h"
#include "src/shared/config/messaging_tcp_config.h"
#include "src/shared/config/osd_cms_config.h"
#include "src/shared/config/tda_config.h"

class Configuration
{
public:
    Configuration();

    OSDCmsConfig *getOsdCmsConfig() const;

    ArpaConfig *getArpaConfig() const;

    AMQPConfig *getAmqpConfig() const;

    MessagingTcpConfig *getTcpMessageConfig() const;

    TDAConfig *getTDAConfig() const;

private:
    OSDCmsConfig *osdCmsConfig;
    ArpaConfig *arpaConfig;

    // TODO: gun cms config

    // TODO: track assignment cms config

    // TODO: engagement correction cms config

    //    AMQPConfig *amqpConfig;
    MessagingTcpConfig *tcpMessageConfig;

    // TODO: gun messaging config

    // TODO: track assigned messaging config

    // TODO: track engagement messaging config

    TDAConfig *tdaConfig;

};

#endif // CONFIGURATION_H
