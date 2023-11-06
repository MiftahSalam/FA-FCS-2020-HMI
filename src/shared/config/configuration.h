#ifndef CONFIGURATION_H
#define CONFIGURATION_H


#include "src/shared/config/amqp_config.h"
#include "src/shared/config/osd_cms_config.h"

class Configuration
{
public:
    Configuration();

    OSDCmsConfig *getOsdCmsConfig() const;

    AMQPConfig *getAmqpConfig() const;

private:
    OSDCmsConfig *osdCmsConfig;
    AMQPConfig *amqpConfig;
};

#endif // CONFIGURATION_H
