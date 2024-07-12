#include "engagement_data_correction_40mm_service.h"
#include "src/shared/common/errors/err_object_creation.h"

EngagementDataCorrection40mmService* EngagementDataCorrection40mmService::instance = nullptr;

EngagementDataCorrection40mmService::EngagementDataCorrection40mmService(
        HttpClientWrapper *parent,
        // TODO: add engagement correction cms config
//            GunCmsConfig *cmsConfig = nullptr,
        EngagementDataCorrectionRepository *repoEngCorr
        ): QObject{parent}, _repoEngCorr(repoEngCorr)
{
    if(parent == nullptr) {
        throw ErrObjectCreation();
    }
}

void EngagementDataCorrection40mmService::onReplyFinished()
{

}

EngagementDataCorrection40mmService *EngagementDataCorrection40mmService::getInstance(
        HttpClientWrapper *httpClient = nullptr,
        // TODO: add gun cms config
//        OSDCmsConfig *cmsConfig = nullptr,
        EngagementDataCorrectionRepository *repoEngCorr
        )
{
    if (instance == nullptr) {
        // TODO: add engagement correction cms config
//        if(cmsConfig == nullptr) {
//            throw ErrObjectCreation();
//        }

        if(httpClient == nullptr) {
            throw ErrObjectCreation();
        }

        if(repoEngCorr == nullptr) {
            throw ErrObjectCreation();
        }

        instance = new EngagementDataCorrection40mmService(httpClient/*, cmsConfig*/, repoEngCorr);
    }

    return instance;
}
