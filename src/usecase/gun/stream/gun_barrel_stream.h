#ifndef GUN_BARREL_STREAM_H
#define GUN_BARREL_STREAM_H

#include "src/domain/gun/repository/gun_feedback_repository.h"
#include "src/infra/messaging/tcp/tcp_messaging_wrapper.h"
#include "src/model/gun/gun_barrel_model.h"
#include "src/shared/config/messaging_tcp_config.h"
#include "src/usecase/gun/stream/gun_stream_base.h"

#include <QObject>

class GunBarrelStream : public QObject, public GunStreamBase<GunBarrelModel>
{
    Q_OBJECT
public:
    GunBarrelStream(GunBarrelStream &other) = delete;
    void operator = (const GunBarrelStream&) = delete;
    static GunBarrelStream *getInstance(
        TcpMessagingOpts *config,
        GunFeedbackRepository *repoGunFback
        );
    // TODO: add members and implementation
protected:
    GunBarrelStream(
        TcpMessagingOpts *_config = nullptr,
        GunFeedbackRepository *_repoGunFback = nullptr
        );

    // GunStreamBase interface
signals:
    void signalDataProcessed(GunBarrelModel data) override;

public:
    BaseError check() override;

private slots:
    void onDataReceived(QByteArray data) override;
    void periodUpdate() override;

private:
    void handleError(const QString &err) override;

    static GunBarrelStream *gunBarrelStream;
    TcpMessagingWrapper *consumer;
    TcpMessagingOpts *cfg;
    GunFeedbackRepository* repoGunFback;
    BaseError currentErr;
};

#endif // GUN_BARREL_STREAM_H
