#include <QString>
#include <QFile>
#include <QSettings>

#include "track_weapon_assign_config.h"
#include "src/shared/common/errors/err_open_file.h"

#ifdef USE_LOG4QT
#include <log4qt/logger.h>
LOG4QT_DECLARE_STATIC_LOGGER(logger, TrackWeaponAssignConfig)
#else
#include <QDebug>
#endif

const QString CONFIG_ENGAGE_CMS_ASSIGN_URL = "engagement/cms/assign/url";
const QString CONFIG_ENGAGE_CMS_RESET_ASSIGN_URL = "engagement/cms/reset-assign/url";
const QString CONFIG_ENGAGE_CMS_CORRECTION_URL = "engagement/cms/correction/url";

TrackWeaponAssignConfig* TrackWeaponAssignConfig::config = nullptr;

TrackWeaponAssignConfig::TrackWeaponAssignConfig()
{

}

TrackWeaponAssignConfig *TrackWeaponAssignConfig::getInstance(const QString path)
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

        config = new TrackWeaponAssignConfig();
        config->setup(path);
    }

    return config;
}

QString TrackWeaponAssignConfig::getAssignUrl() const
{
    return assignUrl;
}

QString TrackWeaponAssignConfig::getResetAssignUrl() const
{
    return resetAssignUrl;
}

QString TrackWeaponAssignConfig::getEngageCorrectionUrl() const
{
    return engageCorrectionUrl;
}

void TrackWeaponAssignConfig::setup(const QString path)
{
    QSettings configFile(path,QSettings::IniFormat);

    assignUrl = configFile.value(CONFIG_ENGAGE_CMS_ASSIGN_URL, "").toString();
    resetAssignUrl = configFile.value(CONFIG_ENGAGE_CMS_RESET_ASSIGN_URL, "").toString();
    engageCorrectionUrl = configFile.value(CONFIG_ENGAGE_CMS_CORRECTION_URL, "").toString();

#ifdef USE_LOG4QT
    logger()->debug()<<Q_FUNC_INFO<<" -> assignUrl: "<<assignUrl;
    logger()->debug()<<Q_FUNC_INFO<<" -> resetAssignUrl: "<<resetAssignUrl;
    logger()->debug()<<Q_FUNC_INFO<<" -> engageCorrectionUrl: "<<engageCorrectionUrl;
#else
    qDebug()<<Q_FUNC_INFO<<"assignUrl"<<assignUrl;
    qDebug()<<Q_FUNC_INFO<<"resetAssignUrl"<<resetAssignUrl;
    qDebug()<<Q_FUNC_INFO<<"engageCorrectionUrl"<<engageCorrectionUrl;
#endif
    }
