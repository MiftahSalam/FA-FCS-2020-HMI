#ifndef TDA_CONFIG_H
#define TDA_CONFIG_H

#include "base_config.h"
#include <QString>

class TDAConfig: public BaseConfig
{
public:
    TDAConfig(TDAConfig &other) = delete;
    void operator = (const TDAConfig&) = delete;
    ~TDAConfig();

    static TDAConfig *getInstance(const QString path);

    QString getCompassStatus() const;
    void setCompassStatus(QString newCompasStatus);
    QString getHeadingMarkerStatus() const;
    void setHeadingMarkerStatus(QString newHeadingMarkerStatus);
    QString getGunCoverageStatus() const;
    void setGunCoverageStatus(QString newGunCoverageStatus);
    QString getGunBarrelStatus() const;
    void setGunBarrelStatus(QString newGunBarrelStatus);
    // BaseConfig interface
public:
    void setup(const QString path) override;
    void saveTDAConfig();

protected:
    TDAConfig();

private:
    static TDAConfig *config;
    QString compassStatus;
    QString headingMarkerStatus;
    QString gunCoverageStatus;
    QString gunBarrelStatus;
};

#endif // TDA_CONFIG_H
