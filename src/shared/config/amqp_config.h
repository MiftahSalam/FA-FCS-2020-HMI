#ifndef OSDCONSUMERCONFIG_H
#define OSDCONSUMERCONFIG_H

#include "base_config.h"

#include <QStringList>


class AMQPConfig: public BaseConfig
{
public:
    AMQPConfig(AMQPConfig &other) = delete;
    void operator=(const AMQPConfig&) = delete;
    ~AMQPConfig();

    static AMQPConfig* getInstance(const QString path);

    QString getServerAddress() const;

protected:
    AMQPConfig();

private:
    static AMQPConfig *config;
    QString serverAddress;

    // BaseConfig interface
    void setup(const QString path) override;
};

#endif // OSDCONSUMERCONFIG_H
