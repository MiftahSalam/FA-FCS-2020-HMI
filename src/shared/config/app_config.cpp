#include "app_config.h"
#include "src/shared/common/errors/err_open_file.h"

#include <QDebug>
#include <QFile>
#include <QSettings>
#include <QDir>

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
    qDebug()<<Q_FUNC_INFO<<"path"<<path;

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
