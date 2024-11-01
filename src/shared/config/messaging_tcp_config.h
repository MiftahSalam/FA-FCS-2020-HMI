#ifndef MESSAGINGTCPCONFIG_H
#define MESSAGINGTCPCONFIG_H

#include "base_config.h"

#include <QMap>

struct TcpMessagingOpts
{
    QString ip;
    quint16 port;
    int delay; //in ms
    int timeout; //in ms
    bool disconnect_rto;
};

class MessagingTcpConfig: public BaseConfig
{
public:
    MessagingTcpConfig(MessagingTcpConfig &other) = delete;
    void operator=(const MessagingTcpConfig&) = delete;
    ~MessagingTcpConfig();

    static MessagingTcpConfig* getInstance(const QString path);

    // BaseConfig interface
    void setup(const QString path) override;

    QMap<QString, TcpMessagingOpts*> getContent() const;

protected:
    MessagingTcpConfig();

private:
    static MessagingTcpConfig *config;
    QMap<QString, TcpMessagingOpts*> content;

    TcpMessagingOpts* generateOpts(QString data);

};

#endif // MESSAGINGTCPCONFIG_H
