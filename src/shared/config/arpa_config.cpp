#include <QDebug>
#include <QFile>
#include <QSettings>

#include "arpa_config.h"
#include "src/shared/common/errors/err_open_file.h"

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
    qDebug()<<Q_FUNC_INFO<<"path"<<path;

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
