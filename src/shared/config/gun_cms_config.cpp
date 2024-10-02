#include <QString>
#include <QFile>
#include <QSettings>

#include "gun_cms_config.h"
#include "src/shared/common/errors/err_open_file.h"

#ifdef USE_LOG4QT
#include <log4qt/logger.h>
LOG4QT_DECLARE_STATIC_LOGGER(logger, GunCmsConfig)
#else
#include <QDebug>
#endif

const QString CONFIG_GUN_CMS_MANUAL_BARREL_URL = "gun/cms/manual_barrel/url";
const QString CONFIG_GUN_CMS_MODE_BARREL_URL = "gun/cms/mode_barrel/url";
const QString CONFIG_GUN_CMS_STATUS_URL = "gun/cms/status/url";

GunCmsConfig* GunCmsConfig::config = nullptr;

GunCmsConfig::GunCmsConfig()
{

}

GunCmsConfig::~GunCmsConfig()
{

}

GunCmsConfig *GunCmsConfig::getInstance(const QString path)
{
#ifdef USE_LOG4QT
        logger()->trace()<<Q_FUNC_INFO<<" -> path; "<<path;
#else
    qDebug()<<Q_FUNC_INFO<<"path"<<path;
#endif

    if(config == nullptr) {
        if (!QFile::exists(path)) {
            throw ErrFileNotFound();
        }

        config = new GunCmsConfig();
        config->setup(path);
    }

    return config;
}

QString GunCmsConfig::getManualBarrelUrl() const
{
    return manualBarrelUrl;
}

QString GunCmsConfig::getModeUrl() const
{
    return modeUrl;
}

QString GunCmsConfig::getStatusUrl() const
{
    return statusUrl;
}

void GunCmsConfig::setup(const QString path)
{
    QSettings configFile(path,QSettings::IniFormat);

    manualBarrelUrl = configFile.value(CONFIG_GUN_CMS_MANUAL_BARREL_URL, "").toString();
    modeUrl = configFile.value(CONFIG_GUN_CMS_MODE_BARREL_URL, "").toString();
    statusUrl = configFile.value(CONFIG_GUN_CMS_STATUS_URL, "").toString();
}
