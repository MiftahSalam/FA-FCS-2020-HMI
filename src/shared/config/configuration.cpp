#include "configuration.h"

#include <QDir>

const QString COMMON_CONFIG_PATH = QDir::homePath()+QDir::separator()+".fa-hmi.conf";

Configuration::Configuration()
{
    osdCmsConfig = OSDCmsConfig::getInstance(COMMON_CONFIG_PATH);
}

OSDCmsConfig *Configuration::getOsdCmsConfig() const
{
    return osdCmsConfig;
}
