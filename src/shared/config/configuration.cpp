#include "configuration.h"

#include <QDir>

const QString COMMON_CONFIG_PATH = QDir::homePath()+QDir::separator()+".fa-hmi.conf";

Configuration::Configuration()
{
    osdCmsConfig = OSDCmsConfig::getInstance(COMMON_CONFIG_PATH);
    arpaConfig = ArpaConfig::getInstance(COMMON_CONFIG_PATH);
    gunCmsConfig = GunCmsConfig::getInstance(COMMON_CONFIG_PATH);
//    amqpConfig = AMQPConfig::getInstance(COMMON_CONFIG_PATH);
    tcpMessageConfig = MessagingTcpConfig::getInstance(COMMON_CONFIG_PATH);
    serialMessageConfig = MessagingSerialConfig::getInstance(COMMON_CONFIG_PATH);
    tdaConfig = TDAConfig::getInstance(COMMON_CONFIG_PATH);
}

OSDCmsConfig *Configuration::getOsdCmsConfig() const
{
    return osdCmsConfig;
}

ArpaConfig *Configuration::getArpaConfig() const
{
    return arpaConfig;
}

MessagingTcpConfig *Configuration::getTcpMessageConfig() const
{
    return tcpMessageConfig;
}

TDAConfig *Configuration::getTDAConfig() const
{
    return tdaConfig;
}

GunCmsConfig *Configuration::getGunCmsConfig() const
{
    return gunCmsConfig;
}

MessagingSerialConfig *Configuration::getSerialMessageConfig() const
{
    return serialMessageConfig;
}

//AMQPConfig *Configuration::getAmqpConfig() const
//{
//    return amqpConfig;
//}
