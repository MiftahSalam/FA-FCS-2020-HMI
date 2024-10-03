#include <QFile>
#include <QSettings>

#include "arpa_config.h"
#include "src/shared/common/errors/err_open_file.h"

#ifdef USE_LOG4QT
#include <log4qt/logger.h>
LOG4QT_DECLARE_STATIC_LOGGER(logger, ArpaConfig)
#else
#include <QDebug>
#endif

const QString CONFIG_ARPA_STALE_TIMEOUT = "arpa/stale_timeout";

ArpaConfig* ArpaConfig::config = nullptr;

ArpaConfig::ArpaConfig()
{

}

ArpaConfig::~ArpaConfig()
{

}

int ArpaConfig::getStaleTimeout() const
{
    return staleTimeout;
}

ArpaConfig* ArpaConfig::getInstance(const QString path) {
#ifdef USE_LOG4QT
    logger()->trace()<<Q_FUNC_INFO<<" -> path; "<<path;
#else
    qDebug()<<Q_FUNC_INFO<<"path"<<path;
#endif

    if(config == nullptr) {
        if (!QFile::exists(path)) {
            throw ErrFileNotFound();
        }

        config = new ArpaConfig();
        config->setup(path);
    }

    return config;
}

void ArpaConfig::setup(const QString path)
{
    QSettings configFile(path,QSettings::IniFormat);

    staleTimeout = configFile.value(CONFIG_ARPA_STALE_TIMEOUT, 10000).toInt();
}
