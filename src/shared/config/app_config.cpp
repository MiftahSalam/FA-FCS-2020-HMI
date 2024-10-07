#include "app_config.h"
#include "src/shared/common/errors/err_open_file.h"

#include <QDebug>
#include <QFile>
#include <QSettings>
#include <QDir>

#ifdef USE_LOG4QT
#include <log4qt/logger.h>
LOG4QT_DECLARE_STATIC_LOGGER(logger, AppConfig)
#else
#include <QDebug>
#endif

const QString CONFIG_ENABLE_TIME_SYNC = "app/time_sync_enable";

AppConfig *AppConfig::config = nullptr;

AppConfig::AppConfig()
{

}

AppConfig::~AppConfig()
{

}

AppConfig *AppConfig::getInstance(const QString path)
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

        config = new AppConfig();
        config->setup(path);
    }

    return config;
}

bool AppConfig::getEnableTimeSync() const
{
    return enableTimeSync;
}

void AppConfig::setEnableTimeSync(bool newEnableTimeSync)
{
    enableTimeSync = newEnableTimeSync;
}

void AppConfig::setup(const QString path)
{
    QSettings configFile(path,QSettings::IniFormat);

    enableTimeSync = configFile.value(CONFIG_ENABLE_TIME_SYNC, "").toBool();
}
