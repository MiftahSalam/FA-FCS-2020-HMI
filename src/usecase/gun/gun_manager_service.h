#ifndef GUNMANAGERSERVICE_H
#define GUNMANAGERSERVICE_H

#include "src/shared/config/gun_cms_config.h"
#include "src/usecase/gun/cms/gun_command_barrel_mode_service.h"
#include "src/usecase/gun/cms/gun_command_barrel_service.h"
#include <QObject>

class GunManagerService : public QObject
{
    Q_OBJECT
public:
    GunManagerService(GunManagerService &other) = delete;
    void operator=(const GunManagerService&) = delete;
    static GunManagerService* getInstance(QObject *parent = nullptr,
            GunCmsConfig *cmsConfig = nullptr,
            GunCommandRepository *cmdRepo= nullptr);

protected:
    GunManagerService(
            QObject *parent = nullptr,
            GunCmsConfig *cmsConfig = nullptr,
            GunCommandBarrelModeService *modeService= nullptr,
            GunCommandBarrelService *barrelService= nullptr
            // TODO : injct gun command status service
//            GunCommandStatus *statusService= nullptr,
            );

private:
    static GunManagerService *gunManagerService;

    GunCmsConfig *_cmsConfig;
    GunCommandBarrelModeService *_modeService;
    GunCommandBarrelService *_barrelService;
    // TODO : injct gun command status service
//            GunCommandStatus *statusService= nullptr,
};

#endif // GUNMANAGERSERVICE_H
