#ifndef ARPACONFIG_H
#define ARPACONFIG_H

#include "base_config.h"

class ArpaConfig: public BaseConfig
{
public:
    ArpaConfig(ArpaConfig &other) = delete;
    void operator=(const ArpaConfig&) = delete;
    ~ArpaConfig();

    static ArpaConfig* getInstance(const QString path);

    int getStaleTimeout() const;

protected:
    ArpaConfig();

private:
    static ArpaConfig *config;
    int staleTimeout;

    // BaseConfig interface
    void setup(const QString path) override;
};

#endif // ARPACONFIG_H
