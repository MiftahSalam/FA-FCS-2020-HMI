#include "configuration.h"

#include <QDir>

const QString COMMON_CONFIG_PATH = QDir::homePath()+QDir::separator()+".fa-hmi.conf";

Configuration::Configuration()
{
    osdCmsConfig = OSDCmsConfig::getInstance(COMMON_CONFIG_PATH);
//    amqpConfig = AMQPConfig::getInstance(COMMON_CONFIG_PATH);
    tcpMessageConfig = MessagingTcpConfig::getInstance(COMMON_CONFIG_PATH);
    tdaConfig = TDAConfig::getInstance(COMMON_CONFIG_PATH);
}

OSDCmsConfig *Configuration::getOsdCmsConfig() const
{
    return osdCmsConfig;
}

MessagingTcpConfig *Configuration::getTcpMessageConfig() const
{
    return tcpMessageConfig;
}

TDAConfig *Configuration::getTDAConfig() const
{
    return tdaConfig;
}
//AMQPConfig *Configuration::getAmqpConfig() const
//{
//    return amqpConfig;
//}
