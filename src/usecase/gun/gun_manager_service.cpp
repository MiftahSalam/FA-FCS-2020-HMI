#include "gun_manager_service.h"
#include "src/shared/common/errors/err_object_creation.h"

GunManagerService* GunManagerService::gunManagerService = nullptr;

GunManagerService::GunManagerService(
        QObject *parent,
        GunCmsConfig *cmsConfig,
        GunCommandBarrelModeService *modeService,
        GunCommandBarrelService *barrelService
        ): QObject(parent), _cmsConfig(cmsConfig), _modeService(modeService), _barrelService(barrelService)
{
}

GunManagerService *GunManagerService::getInstance(
        QObject *parent,
        GunCmsConfig *cmsConfig,
        GunCommandRepository *cmdRepo
        )
{
    if (gunManagerService == nullptr) {
        if(cmsConfig == nullptr) {
            throw ErrObjectCreation();
        }

        if(cmdRepo == nullptr) {
            throw ErrObjectCreation();
        }

        GunCommandBarrelModeService* modeService = GunCommandBarrelModeService::getInstance(
                    new HttpClientWrapper(),
                    cmsConfig,
                    cmdRepo
                    );
        GunCommandBarrelService* barrelService = GunCommandBarrelService::getInstance(
                    new HttpClientWrapper(),
                    cmsConfig,
                    cmdRepo
                    );
        // TODO : gun command status service
        gunManagerService = new GunManagerService(parent, cmsConfig, modeService, barrelService);
    }

    return gunManagerService;
}
