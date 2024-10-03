#include <QString>
#include <QFile>
#include <QSettings>

#include "osd_cms_config.h"
#include "src/shared/common/errors/err_open_file.h"

#ifdef USE_LOG4QT
#include <log4qt/logger.h>
LOG4QT_DECLARE_STATIC_LOGGER(logger, OSDCmsConfig)
#else
#include <QDebug>
#endif

const QString CONFIG_OSD_CMS_MANUAL_DATA_URL = "osd/cms/manual_data/url";
const QString CONFIG_OSD_CMS_MODE_URL = "osd/cms/mode/url";

OSDCmsConfig* OSDCmsConfig::config = nullptr;

OSDCmsConfig::OSDCmsConfig()
{

}

OSDCmsConfig::~OSDCmsConfig()
{

}

QString OSDCmsConfig::getManualDataUrl() const
{
    return manualDataUrl;
}

QString OSDCmsConfig::getModeUrl() const
{
    return modeUrl;
}

OSDCmsConfig* OSDCmsConfig::getInstance(const QString path) {
#ifdef USE_LOG4QT
    logger()->trace()<<Q_FUNC_INFO<<" -> path; "<<path;
#else
    qDebug()<<Q_FUNC_INFO<<"path"<<path;
#endif

    if(config == nullptr) {
        if (!QFile::exists(path)) {
            throw ErrFileNotFound();
        }

        config = new OSDCmsConfig();
        config->setup(path);
    }

    return config;
}


void OSDCmsConfig::setup(const QString path)
{
    QSettings configFile(path,QSettings::IniFormat);

    manualDataUrl = configFile.value(CONFIG_OSD_CMS_MANUAL_DATA_URL, "").toString();
    modeUrl = configFile.value(CONFIG_OSD_CMS_MODE_URL, "").toString();
}
