#include <QFile>
#include "qdebug.h"
#include <QSettings>

#include "amqp_config.h"
#include "src/shared/common/errors/err_open_file.h"

const QString CONFIG_AMQP_URL= "amqp/url";

AMQPConfig* AMQPConfig::config = nullptr;
AMQPConfig::AMQPConfig()
{

}

QString AMQPConfig::getServerAddress() const
{
    return serverAddress;
}

AMQPConfig::~AMQPConfig()
{

}

AMQPConfig *AMQPConfig::getInstance(const QString path)
{
    qDebug()<<Q_FUNC_INFO<<"path"<<path;

    if(config == nullptr) {
        if (!QFile::exists(path)) {
            throw ErrFileNotFound();
        }

        config = new AMQPConfig();
        config->setup(path);
    }

    return config;

}

void AMQPConfig::setup(const QString path)
{
    QSettings configFile(path,QSettings::IniFormat);

    serverAddress = configFile.value(CONFIG_AMQP_URL, "").toString();

}
