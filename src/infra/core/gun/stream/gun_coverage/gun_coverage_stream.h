#ifndef GUN_COVERAGE_STREAM_H
#define GUN_COVERAGE_STREAM_H

#include "src/infra/messaging/IOSDStream.h"
#include "src/infra/messaging/tcp/tcp_messaging_wrapper.h"
#include "src/infra/core/gun/stream/gun_coverage/gun_coverage_model.h"
#include "src/shared/config/messaging_tcp_config.h"

#include <QObject>

class GunCoverageStream : public QObject, public IOSDStream<GunCoverageModel>
{
    Q_OBJECT
public:
    GunCoverageStream(GunCoverageStream &other) = delete;
    void operator=(const GunCoverageStream&) = delete;
    static GunCoverageStream *getInstance(
        TcpMessagingOpts *config);

    BaseError check() override;

signals:
    void signalDataProcessed(GunCoverageModel data) override;

protected:
    GunCoverageStream(
        TcpMessagingOpts *config = nullptr
        );

private slots:
    void onDataReceived(QByteArray data) override;

private:
    static GunCoverageStream *gunCoverageStream;

    TcpMessagingWrapper *consumer;
    TcpMessagingOpts *cfg;

    BaseError currentErr;

    void handleError(const QString &err) override;
};

#endif // GUN_COVERAGE_STREAM_H
