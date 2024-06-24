#include "messaging_tcp_config.h"
#include "src/shared/common/errors/err_messaging.h"
#include "src/shared/common/errors/err_open_file.h"

#include <QDebug>
#include <QFile>
#include <QSettings>

const QString CONFIG_MESSAGING= "messaging";

MessagingTcpConfig* MessagingTcpConfig::config = nullptr;

MessagingTcpConfig::MessagingTcpConfig()
{

}

QMap<QString, TcpMessagingOpts*> MessagingTcpConfig::getContent() const
{
    return content;
}

MessagingTcpConfig *MessagingTcpConfig::getInstance(const QString path)
{
    qDebug()<<Q_FUNC_INFO<<"path"<<path;

    if(config == nullptr) {
        if (!QFile::exists(path)) {
            throw ErrFileNotFound();
        }

        config = new MessagingTcpConfig();
        config->setup(path);
    }

    return config;

}

void MessagingTcpConfig::setup(const QString path)
{
    QSettings configFile(path,QSettings::IniFormat);

    configFile.beginGroup(CONFIG_MESSAGING);
    foreach (auto key, configFile.childGroups()) {
        qDebug()<<Q_FUNC_INFO<<"key:"<<key<<"value:"<<configFile.value(key, "");

        TcpMessagingOpts* opts = new TcpMessagingOpts();

        configFile.beginGroup(key);
        foreach (auto key1, configFile.childKeys()) {
            qDebug()<<Q_FUNC_INFO<<"child key1:"<<key1<<"value1:"<<configFile.value(key1, "");
            if (key1 == "delay") {
                opts->delay = configFile.value(key1, "").toUInt();
            } else if (key1 == "timeout") {
                opts->timeout = configFile.value(key1, "").toUInt();
            }
        }
        configFile.endGroup();
        configFile.beginGroup(key);
        foreach (auto key1, configFile.childGroups()) {
            qDebug()<<Q_FUNC_INFO<<"group key1:"<<key1<<"value1:"<<configFile.value(key1, "");
            configFile.beginGroup(key1);
            foreach (auto key2, configFile.childKeys()) {
                qDebug()<<Q_FUNC_INFO<<"child key2:"<<key2<<"value2:"<<configFile.value(key2, "");
                if (key2 == "host") {
                    opts->ip = configFile.value(key2, "").toString();
                } else if (key2 == "port") {
                    opts->port = configFile.value(key2, "").toUInt();
                }
            }
            configFile.endGroup();
            content[key] = opts;
        }
        configFile.endGroup();

    }
    configFile.endGroup();

    foreach (auto key, content.keys()) {
        TcpMessagingOpts *val = content.value(key);
        qDebug()<<Q_FUNC_INFO<<"content:"<<key<<"ip:"<<val->ip<<"port:"<<val->port<<"delay:"<<val->delay<<"timeout:"<<val->timeout;
    }
}

TcpMessagingOpts *MessagingTcpConfig::generateOpts(QString data)
{
    QStringList dataList = data.split(":",QString::SkipEmptyParts);
    qDebug()<<Q_FUNC_INFO<<"dataList:"<<dataList;
    if (dataList.size() != 3) {
        throw ErrMessagingTcpConfig();
    }

    TcpMessagingOpts* opts = new TcpMessagingOpts();
    opts->ip = dataList.at(0);
    opts->port = dataList.at(1).toUShort();
    opts->delay = dataList.at(2).toInt();

    return opts;
}

MessagingTcpConfig::~MessagingTcpConfig()
{

}
