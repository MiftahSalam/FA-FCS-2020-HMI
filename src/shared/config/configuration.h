#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include "src/shared/config/amqp_config.h"
#include "src/shared/config/app_config.h"
#include "src/shared/config/arpa_config.h"
#include "src/shared/config/gun_cms_config.h"
#include "src/shared/config/messaging_serial_config.h"
#include "src/shared/config/messaging_tcp_config.h"
#include "src/shared/config/osd_cms_config.h"
#include "src/shared/config/tda_config.h"
#include "src/shared/config/track_weapon_assign_config.h"

class Configuration
{
public:
    Configuration();

    OSDCmsConfig *getOsdCmsConfig() const;

    ArpaConfig *getArpaConfig() const;

    AMQPConfig *getAmqpConfig() const;

    MessagingTcpConfig *getTcpMessageConfig() const;

    TDAConfig *getTDAConfig() const;

    GunCmsConfig *getGunCmsConfig() const;

    MessagingSerialConfig *getSerialMessageConfig() const;

    TrackWeaponAssignConfig *getTrackWeaponAssignCmsConfig() const;

    AppConfig *getAppConfig() const;

private:
    OSDCmsConfig *osdCmsConfig;
    AppConfig *appConfig;
    ArpaConfig *arpaConfig;
    GunCmsConfig *gunCmsConfig;
    TrackWeaponAssignConfig *trackWeaponAssignCmsConfig;

    //    AMQPConfig *amqpConfig;
    MessagingTcpConfig *tcpMessageConfig;
    MessagingSerialConfig *serialMessageConfig;

    TDAConfig *tdaConfig;
};

#endif // CONFIGURATION_H
