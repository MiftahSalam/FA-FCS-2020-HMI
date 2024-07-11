#ifndef GUNCOMMANDSERVICE_H
#define GUNCOMMANDSERVICE_H

#include "src/domain/gun/repository/gun_command_repository.h"
#include "src/infra/http/http_client_wrapper.h"
#include <QObject>

class GunCommandService : public QObject
{
    Q_OBJECT
public:
    GunCommandService(GunCommandService &other) = delete;
    void operator=(const GunCommandService&) = delete;
    static GunCommandService* getInstance(
            HttpClientWrapper *httpClient,
            // TODO: add gun cms config
//            GunCmsConfig *cmsConfig = nullptr,
            GunCommandRepository *repoGunCmd = nullptr
            );

    // TODO: add request methods implementation
//    void setBarrel(OSDSetPositionRequest request);
    //    void setStatus(OSDSetPositionRequest request);

signals:
//    void signal_setBarrelResponse(BaseResponse<PositionModel> response);
    //    void signal_setStatusResponse(BaseResponse<PositionModel> response);

protected:
    GunCommandService(
            HttpClientWrapper *parent = nullptr,
            // TODO: add gun cms config
//            GunCmsConfig *cmsConfig = nullptr,
            GunCommandRepository *repoGunCmd = nullptr
            );

private slots:
    void onReplyFinished();

private:
    static GunCommandService *instance;
    // TODO: add gun cms config
//    OSDCmsConfig *cfgCms;
    GunCommandRepository* _repoGunCmd;

//    BaseResponse<PositionModel> toResponse(QByteArray raw) override;
//    BaseResponse<PositionModel> errorResponse(QNetworkReply::NetworkError err) override;
};

#endif // GUNCOMMANDSERVICE_H
