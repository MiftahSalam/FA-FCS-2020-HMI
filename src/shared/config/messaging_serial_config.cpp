#include "messaging_serial_config.h"
#include "src/shared/common/errors/err_open_file.h"

#include <QDebug>
#include <QFile>
#include <QSettings>

const QString CONFIG_MESSAGING= "messaging";

MessagingSerialConfig* MessagingSerialConfig::config = nullptr;

MessagingSerialConfig::MessagingSerialConfig()
{

}

QMap<QString, SerialMessagingOpts *> MessagingSerialConfig::getContent() const
{
    return content;
}

void MessagingSerialConfig::setup(const QString path)
{
    QSettings configFile(path,QSettings::IniFormat);

    configFile.beginGroup(CONFIG_MESSAGING);
    foreach (auto key, configFile.childGroups()) {
        qDebug()<<Q_FUNC_INFO<<"key:"<<key<<"value:"<<configFile.value(key, "");

        SerialMessagingOpts* opts = new SerialMessagingOpts();

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
                if (key2 == "portname") {
                    opts->portname = configFile.value(key2, "").toString();
                } else if (key2 == "baudrate") {
                    opts->baudrate = configFile.value(key2, "").toString();
                } else if (key2 == "databits") {
                    opts->databits = configFile.value(key2, "").toString();
                } else if (key2 == "stopbit") {
                    opts->stopbits = configFile.value(key2, "").toString();
                } else if (key2 == "parity") {
                    opts->parity = configFile.value(key2, "").toString();
                } else if (key2 == "flow") {
                    opts->flowControls = configFile.value(key2, "").toString();
                } else {
                    qDebug()<<Q_FUNC_INFO<<"invalid child key2:"<<key2<<"value2:"<<configFile.value(key2, "");
                }
            }
            configFile.endGroup();
            content[key] = opts;
        }
        configFile.endGroup();

    }
    configFile.endGroup();

    foreach (auto key, content.keys()) {
        SerialMessagingOpts *val = content.value(key);
        qDebug()<<Q_FUNC_INFO<<"content:"<<key
               <<"portname:"<<val->portname
              <<"baudrate:"<<val->baudrate
             <<"databits:"<<val->databits
            <<"stopbit:"<<val->stopbits
           <<"parity:"<<val->parity
          <<"flow:"<<val->flowControls
         <<"delay:"<<val->delay
        <<"timeout:"<<val->timeout;
    }
}

MessagingSerialConfig *MessagingSerialConfig::getInstance(const QString path)
{
    qDebug()<<Q_FUNC_INFO<<"path"<<path;

    if(config == nullptr) {
        if (!QFile::exists(path)) {
            throw ErrFileNotFound();
        }

        config = new MessagingSerialConfig();
        config->setup(path);
    }

    return config;

}
