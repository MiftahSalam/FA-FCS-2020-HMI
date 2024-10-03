#include "messaging_tcp_config.h"
#include "src/shared/common/errors/err_messaging.h"
#include "src/shared/common/errors/err_open_file.h"

#include <QFile>
#include <QSettings>

#ifdef USE_LOG4QT
#include <log4qt/logger.h>
LOG4QT_DECLARE_STATIC_LOGGER(logger, MessagingTcpConfig)
#else
#include <QDebug>
#endif

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
#ifdef USE_LOG4QT
    logger()->trace()<<Q_FUNC_INFO<<" -> path; "<<path;
#else
    qDebug()<<Q_FUNC_INFO<<"path"<<path;
#endif

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
#ifdef USE_LOG4QT
        logger()->trace()<<Q_FUNC_INFO<<" -> key: "<<key<<", value: "<<configFile.value(key, "").toString();
#else
        qDebug()<<Q_FUNC_INFO<<"key:"<<key<<"value:"<<configFile.value(key, "");
#endif

        TcpMessagingOpts* opts = new TcpMessagingOpts();

        configFile.beginGroup(key);
        foreach (auto key1, configFile.childKeys()) {
#ifdef USE_LOG4QT
            logger()->trace()<<Q_FUNC_INFO<<" -> child key1: "<<key1<<", value1: "<<configFile.value(key1, "").toString();
#else
            qDebug()<<Q_FUNC_INFO<<"child key1:"<<key1<<"value1:"<<configFile.value(key1, "");
#endif
            if (key1 == "delay") {
                opts->delay = configFile.value(key1, "").toUInt();
            } else if (key1 == "timeout") {
                opts->timeout = configFile.value(key1, "").toUInt();
            }
        }
        configFile.endGroup();
        configFile.beginGroup(key);
        foreach (auto key1, configFile.childGroups()) {
#ifdef USE_LOG4QT
            logger()->trace()<<Q_FUNC_INFO<<" -> group key1: "<<key1<<", value1: "<<configFile.value(key1, "").toString();
#else
            qDebug()<<Q_FUNC_INFO<<"group key1:"<<key1<<"value1:"<<configFile.value(key1, "");
#endif
            configFile.beginGroup(key1);
            foreach (auto key2, configFile.childKeys()) {
#ifdef USE_LOG4QT
                logger()->trace()<<Q_FUNC_INFO<<" -> child key2: "<<key2<<", value2: "<<configFile.value(key2, "").toString();
#else
                qDebug()<<Q_FUNC_INFO<<"child key2:"<<key2<<"value2:"<<configFile.value(key2, "");
#endif
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
#ifdef USE_LOG4QT
        logger()->debug()<<Q_FUNC_INFO<<" -> content: "<<key
                        <<". ip: "<<val->ip
                       <<", port: "<<val->port
                      <<", delay: "<<val->delay
                     <<", timeout: "<<val->timeout;
#else
        qDebug()<<Q_FUNC_INFO<<"content:"<<key<<"ip:"<<val->ip<<"port:"<<val->port<<"delay:"<<val->delay<<"timeout:"<<val->timeout;
#endif
    }
}

TcpMessagingOpts *MessagingTcpConfig::generateOpts(QString data)
{
    QStringList dataList = data.split(":",Qt::SkipEmptyParts);
#ifdef USE_LOG4QT
    foreach (auto lst, dataList) {
        logger()->trace()<<Q_FUNC_INFO<<" -> list: "<<lst;
    }
#else
    qDebug()<<Q_FUNC_INFO<<"dataList:"<<dataList;
#endif
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
