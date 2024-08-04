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

    bool getCompassStatus() const;
    void setCompassStatus(bool newCompasStatus);
    bool getHeadingMarkerStatus() const;
    void setHeadingMarkerStatus(bool newHeadingMarkerStatus);
    bool getGunCoverageStatus() const;
    void setGunCoverageStatus(bool newGunCoverageStatus);
    bool getGunBarrelStatus() const;
    void setGunBarrelStatus(bool newGunBarrelStatus);
    // BaseConfig interface
public:
    void setup(const QString path) override;
    void saveTDAConfig();

protected:
    TDAConfig();

private:
    static TDAConfig *config;
    bool compassStatus;
    bool headingMarkerStatus;
    bool gunCoverageStatus;
    bool gunBarrelStatus;
};

#endif // TDA_CONFIG_H
