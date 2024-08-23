#ifndef GUNCMSCONFIG_H
#define GUNCMSCONFIG_H

#include "base_config.h"

class GunCmsConfig: public BaseConfig
{
public:
    GunCmsConfig(GunCmsConfig &other) = delete;
    void operator=(const GunCmsConfig&) = delete;
    ~GunCmsConfig();

    static GunCmsConfig* getInstance(const QString path);

    QString getManualBarrelUrl() const;
    QString getModeUrl() const;
    QString getStatusUrl() const;

protected:
    GunCmsConfig();

private:
    static GunCmsConfig *config;
    QString manualBarrelUrl;
    QString modeUrl;
    QString statusUrl;

    // BaseConfig interface
    void setup(const QString path) override;
};

#endif // GUNCMSCONFIG_H
