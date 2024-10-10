#include "app_config.h"
#include "src/shared/common/errors/err_open_file.h"

#include <QDebug>
#include <QFile>
#include <QSettings>
#include <QDir>
#include <QDateTime>

#ifdef USE_LOG4QT
#include <log4qt/logger.h>
LOG4QT_DECLARE_STATIC_LOGGER(logger, AppConfig)
#else
#include <QDebug>
#endif

const QString CONFIG_ENABLE_TIME_SYNC = "app/time_sync_enable";
const QString CONFIG_TIME_DISPLAY_FORMAT = "app/time_display_format";

AppConfig *AppConfig::config = nullptr;

AppConfig::AppConfig()
{

}

QString AppConfig::getTimeDislayFormat() const
{
    return timeDislayFormat;
}

void AppConfig::setTimeDislayFormat(const QString &newTimeDislayFormat)
{
    timeDislayFormat = newTimeDislayFormat;
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
    timeDislayFormat = configFile.value(CONFIG_TIME_DISPLAY_FORMAT, "").toString();

    QDateTime now = QDateTime::fromSecsSinceEpoch(QDateTime::currentSecsSinceEpoch());
    QDateTime formatCheck = QDateTime::fromString(now.toString(timeDislayFormat), timeDislayFormat);

#ifdef USE_LOG4QT
    logger()->trace()<<Q_FUNC_INFO<<" -> date time format: "<<timeDislayFormat;
    logger()->trace()<<Q_FUNC_INFO<<" -> now: "<<now.toString();
    logger()->trace()<<Q_FUNC_INFO<<" -> formatCheck: "<<formatCheck.toString();
#else
    qDebug()<<Q_FUNC_INFO<<"date time format:"<<timeDislayFormat;
    qDebug()<<Q_FUNC_INFO<<"now:"<<now;
    qDebug()<<Q_FUNC_INFO<<"formatCheck:"<<formatCheck;
#endif

    if (!formatCheck.isValid() || now != formatCheck) {
#ifdef USE_LOG4QT
    logger()->trace()<<Q_FUNC_INFO<<" -> invalid date time format; "<<timeDislayFormat;
#else
    qFatal("invalid date time format: %s", timeDislayFormat.toUtf8().constData());
#endif
    }
}
