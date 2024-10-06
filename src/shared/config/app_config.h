#ifndef APPCONFIG_H
#define APPCONFIG_H

#include "base_config.h"

class AppConfig: public BaseConfig
{
public:
    AppConfig(AppConfig &other) = delete;
    void operator = (const AppConfig&) = delete;
    ~AppConfig();

    static AppConfig *getInstance(const QString path);

    bool getEnableTimeSync() const;
    void setEnableTimeSync(bool newEnableTimeSync);

protected:
    AppConfig();

private:
    static AppConfig *config;

    bool enableTimeSync;

    // BaseConfig interface
    void setup(const QString path) override;
};

#endif // APPCONFIG_H
