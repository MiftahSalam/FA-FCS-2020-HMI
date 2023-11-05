#include <QDebug>
#include <QString>
#include <QFile>
#include <QSettings>

#include "osd_cms_config.h"
#include "src/shared/common/errors/err_open_file.h"

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
    qDebug()<<Q_FUNC_INFO<<"path"<<path;

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
