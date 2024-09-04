#ifndef MESSAGINGSERIALCONFIG_H
#define MESSAGINGSERIALCONFIG_H

#include "base_config.h"

#include <QMap>
#include <QSerialPort>

struct SerialMessagingOpts
{
    QString portname;
    QString baudrate;
    QString databits;
    QString stopbits;
    QString parity;
    QString flowControls;
    int delay; //in ms
    int timeout; //in ms
};

class MessagingSerialConfig: public BaseConfig
{
public:
    MessagingSerialConfig(MessagingSerialConfig &other) = delete;
    void operator=(const MessagingSerialConfig&) = delete;
    ~MessagingSerialConfig();

    static MessagingSerialConfig* getInstance(const QString path);

    // BaseConfig interface
    void setup(const QString path) override;

    QMap<QString, SerialMessagingOpts *> getContent() const;

protected:
    MessagingSerialConfig();

private:
    static MessagingSerialConfig *config;
    QMap<QString, SerialMessagingOpts*> content;

    SerialMessagingOpts* generateOpts(QString data);

};
#endif // MESSAGINGSERIALCONFIG_H
