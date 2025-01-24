#include "osd_service.h"
#include "src/shared/common/errors/err_object_creation.h"

OSDService* OSDService::instance = nullptr;

OSDService::OSDService(QObject *parent, OSDRepository *osdRepo, OSDCMS* osdCms)
    : QObject{parent}, repoOsd(osdRepo), cmsOsd(osdCms)
{
    connect(cmsOsd->getServiceOSDCMSMode(), &OSDCMSInputMode::signal_setModeResponse, this, &OSDService::onUpdateInputMode);

}

OSDService* OSDService::getInstance(QObject* parent, OSDRepository *osdRepo, OSDCMS* osdCms) {
    if(instance == nullptr) {
        if(osdRepo == nullptr) {
            throw ErrObjectCreation();
        }

        if(osdCms == nullptr) {
            throw ErrObjectCreation();
        }

        instance = new OSDService(parent, osdRepo, osdCms);
    }

    return instance;
}

void OSDService::onUpdateManualDataGyro(BaseResponse<GyroModel> response)
{

}

void OSDService::onUpdateInputMode(const QString datafisis, BaseResponse<InputModeModel> response, bool needConfirm)
{

}

OSDService::~OSDService() {

}
