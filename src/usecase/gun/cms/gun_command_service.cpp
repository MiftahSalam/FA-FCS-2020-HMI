#include "gun_command_service.h"
#include "src/shared/common/errors/err_object_creation.h"

GunCommandService* GunCommandService::instance = nullptr;

GunCommandService::GunCommandService(
        HttpClientWrapper *parent,
        // TODO: add gun cms config
//        OSDCmsConfig *cmsConfig,
        GunCommandRepository *repoGunCmd
        ): QObject{parent}, _repoGunCmd(repoGunCmd)
{
    if(parent == nullptr) {
        throw ErrObjectCreation();
    }
}

void GunCommandService::onReplyFinished()
{

}

GunCommandService *GunCommandService::getInstance(
        HttpClientWrapper *httpClient = nullptr,
        // TODO: add gun cms config
//        OSDCmsConfig *cmsConfig = nullptr,
        GunCommandRepository *repoGunCmd
        )
{
    if (instance == nullptr) {
        // TODO: add gun cms config
//        if(cmsConfig == nullptr) {
//            throw ErrObjectCreation();
//        }

        if(httpClient == nullptr) {
            throw ErrObjectCreation();
        }

        if(repoGunCmd == nullptr) {
            throw ErrObjectCreation();
        }

        instance = new GunCommandService(httpClient/*, cmsConfig*/, repoGunCmd);
    }

    return instance;
}
