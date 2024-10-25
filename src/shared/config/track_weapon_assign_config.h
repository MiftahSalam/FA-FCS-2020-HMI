#ifndef TRACKWEAPONASSIGNCONFIG_H
#define TRACKWEAPONASSIGNCONFIG_H

#include "base_config.h"

class TrackWeaponAssignConfig: public BaseConfig
{
public:
    TrackWeaponAssignConfig(TrackWeaponAssignConfig &other) = delete;
    void operator=(const TrackWeaponAssignConfig&) = delete;
    ~TrackWeaponAssignConfig();

    static TrackWeaponAssignConfig* getInstance(const QString path);

    QString getAssignUrl() const;
    QString getEngageCorrectionUrl() const;
    QString getResetAssignUrl() const;

    QString getAssignHBUrl() const;

protected:
    TrackWeaponAssignConfig();

private:
    static TrackWeaponAssignConfig *config;

    QString assignUrl;
    QString assignHBUrl;
    QString resetAssignUrl;
    QString engageCorrectionUrl;

    // BaseConfig interface
    void setup(const QString path) override;
};

#endif // TRACKWEAPONASSIGNCONFIG_H
