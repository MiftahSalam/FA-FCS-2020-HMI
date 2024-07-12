#ifndef ENGAGEMENTDATACORRECTION40MMSERVICE_H
#define ENGAGEMENTDATACORRECTION40MMSERVICE_H

#include "src/domain/engagement/repository/engagement_data_correction_repository.h"
#include "src/infra/http/http_client_wrapper.h"
#include <QObject>

class EngagementDataCorrection40mmService : public QObject
{
    Q_OBJECT
public:
    EngagementDataCorrection40mmService(EngagementDataCorrection40mmService &other) = delete;
    void operator=(const EngagementDataCorrection40mmService&) = delete;
    static EngagementDataCorrection40mmService* getInstance(
            HttpClientWrapper *httpClient,
            // TODO: add engagement correction cms config
//            GunCmsConfig *cmsConfig = nullptr,
            EngagementDataCorrectionRepository *repoEngCorr = nullptr
            );

    // TODO: add request methods implementation
//    void setCorrection(EngagementCorrectionSetRequest request);

signals:
//    void signal_setCorrectionResponse(BaseResponse<EngagementCorrectionSetRequest> response);

protected:
    EngagementDataCorrection40mmService(
            HttpClientWrapper *parent = nullptr,
            // TODO: add engagement correction cms config
//            GunCmsConfig *cmsConfig = nullptr,
            EngagementDataCorrectionRepository *repoEngCorr = nullptr
            );

private slots:
    void onReplyFinished();

private:
    static EngagementDataCorrection40mmService *instance;
    // TODO: add engagement correction cms config
    EngagementDataCorrectionRepository* _repoEngCorr;

//    BaseResponse<PositionModel> toResponse(QByteArray raw) override;
//    BaseResponse<PositionModel> errorResponse(QNetworkReply::NetworkError err) override;

};

#endif // ENGAGEMENTDATACORRECTION40MMSERVICE_H
