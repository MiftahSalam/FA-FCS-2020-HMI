#ifndef GUN_FEEDBACK_BARREL_STREAM_H
#define GUN_FEEDBACK_BARREL_STREAM_H

#include "src/domain/gun/repository/gun_feedback_repository.h"
#include "src/infra/messaging/tcp/tcp_messaging_wrapper.h"
#include "src/infra/core/gun/stream/gun_barrel/gun_feedback_barrel_model.h"
#include "src/shared/config/messaging_tcp_config.h"
#include "src/infra/core/gun/stream/gun_stream_base.h"

#include <QObject>

class GunFeedbackBarrelStream : public QObject, public GunStreamBase<GunFeedbackBarrelModel>
{
    Q_OBJECT
public:
    GunFeedbackBarrelStream(GunFeedbackBarrelStream &other) = delete;
    void operator = (const GunFeedbackBarrelStream&) = delete;
    static GunFeedbackBarrelStream *getInstance(
        TcpMessagingOpts *config,
        GunFeedbackRepository *repoGunFback
        );

    void resetBarrel();

protected:
    GunFeedbackBarrelStream(
        TcpMessagingOpts *_config = nullptr,
        GunFeedbackRepository *_repoGunFback = nullptr
        );

    // GunStreamBase interface
signals:
    void signalDataProcessed(GunFeedbackBarrelModel data) override;

public:
    BaseError check() override;

private slots:
    void onDataReceived(QByteArray data) override;
    void periodUpdate() override;

private:
    void handleError(const QString &err) override;

    static GunFeedbackBarrelStream *gunBarrelStream;
    TcpMessagingWrapper *consumer;
    TcpMessagingOpts *cfg;
    GunFeedbackRepository* repoGunFback;
    BaseError currentErr;
};

#endif // GUN_FEEDBACK_BARREL_STREAM_H
