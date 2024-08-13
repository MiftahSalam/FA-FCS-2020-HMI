#ifndef GUN_COVERAGE_STREAM_H
#define GUN_COVERAGE_STREAM_H

#include "src/domain/gun/repository/gun_coverage_repository.h"
#include "src/infra/messaging/tcp/tcp_messaging_wrapper.h"
#include "src/model/gun/cms/gun_coverage_model.h"
#include "src/shared/config/messaging_tcp_config.h"
#include "src/usecase/gun/stream/gun_stream_base.h"

#include <QObject>
// #include "src/model/gun/

class GunCoverageStream : public QObject, public GunStreamBase<GunCoverageModel>
{
    Q_OBJECT
public:
    GunCoverageStream(GunCoverageStream &other) = delete;
    void operator=(const GunCoverageStream&) = delete;
    static GunCoverageStream *getInstance(
        TcpMessagingOpts *config,
        GunCoverageRepository *_repoGunCov
        );

    BaseError check() override;

signals:
    void signalDataProcessed(GunCoverageModel data) override;

protected:
    GunCoverageStream(
        TcpMessagingOpts *config = nullptr,
        GunCoverageRepository *repoGunCov = nullptr
        );

private slots:
    void onDataReceived(QByteArray data) override;

private:
    static GunCoverageStream *gunCoverageStream;
    TcpMessagingWrapper *consumer;
    TcpMessagingOpts *cfg;
    GunCoverageRepository* _repoGunCov;

    BaseError currentErr;

    void handleError(const QString &err) override;

};

#endif // GUN_COVERAGE_STREAM_H
