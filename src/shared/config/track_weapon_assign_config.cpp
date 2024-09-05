#include <QDebug>
#include <QString>
#include <QFile>
#include <QSettings>

#include "track_weapon_assign_config.h"
#include "src/shared/common/errors/err_open_file.h"

const QString CONFIG_ENGAGE_CMS_ASSIGN_URL = "engagement/cms/assign/url";
const QString CONFIG_ENGAGE_CMS_CORRECTION_URL = "engagement/cms/correction/url";

TrackWeaponAssignConfig* TrackWeaponAssignConfig::config = nullptr;

TrackWeaponAssignConfig::TrackWeaponAssignConfig()
{

}

TrackWeaponAssignConfig *TrackWeaponAssignConfig::getInstance(const QString path)
{
    qDebug()<<Q_FUNC_INFO<<"path"<<path;

    if(config == nullptr) {
        if (!QFile::exists(path)) {
            throw ErrFileNotFound();
        }

        config = new TrackWeaponAssignConfig();
        config->setup(path);
    }

    return config;
}

QString TrackWeaponAssignConfig::getAssignUrl() const
{
    return assignUrl;
}

QString TrackWeaponAssignConfig::getEngageCorrectionUrl() const
{
    return engageCorrectionUrl;
}

void TrackWeaponAssignConfig::setup(const QString path)
{
    QSettings configFile(path,QSettings::IniFormat);

    assignUrl = configFile.value(CONFIG_ENGAGE_CMS_ASSIGN_URL, "").toString();
    engageCorrectionUrl = configFile.value(CONFIG_ENGAGE_CMS_CORRECTION_URL, "").toString();

    qDebug()<<Q_FUNC_INFO<<"assignUrl"<<assignUrl;
    qDebug()<<Q_FUNC_INFO<<"engageCorrectionUrl"<<engageCorrectionUrl;
}
