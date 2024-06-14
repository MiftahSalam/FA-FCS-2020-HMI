#ifndef OSDCMSCONFIG_H
#define OSDCMSCONFIG_H

#include <QString>

#include "base_config.h"
#include <qdebug.h>

class OSDCmsConfig: public BaseConfig
{
public:
    OSDCmsConfig(OSDCmsConfig &other) = delete;
    void operator=(const OSDCmsConfig&) = delete;
    ~OSDCmsConfig();

    static OSDCmsConfig* getInstance(const QString path);
//    qDebug() << OSDCmsConfig;
//    qDebug()<< OSDCmsConfig ;

    QString getManualDataUrl() const;
    QString getModeUrl() const;

protected:
    OSDCmsConfig();

private:
    static OSDCmsConfig *config;
    QString manualDataUrl;
    QString modeUrl;

    // BaseConfig interface
    void setup(const QString path) override;
};

#endif // OSDCMSCONFIG_H
