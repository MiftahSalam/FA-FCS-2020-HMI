#ifndef GUN_FEEDBACK_BARREL_STREAM_H
#define GUN_FEEDBACK_BARREL_STREAM_H

#include "src/infra/messaging/IOSDStream.h"
#include "src/infra/messaging/tcp/tcp_messaging_wrapper.h"
#include "src/infra/core/gun/stream/gun_barrel/gun_feedback_barrel_model.h"
#include "src/shared/config/messaging_tcp_config.h"

#include <QObject>

class GunFeedbackBarrelStream : public QObject, public IOSDStream<GunFeedbackBarrelModel>
{
    Q_OBJECT
public:
    GunFeedbackBarrelStream(GunFeedbackBarrelStream &other) = delete;
    void operator = (const GunFeedbackBarrelStream&) = delete;
    static GunFeedbackBarrelStream *getInstance(
        TcpMessagingOpts *config
        );

    BaseError check() override;

signals:
    void signalDataProcessed(GunFeedbackBarrelModel data) override;

protected:
    GunFeedbackBarrelStream(
        TcpMessagingOpts *_config = nullptr
        );

private slots:
    void onDataReceived(QByteArray data) override;

private:
    static GunFeedbackBarrelStream *gunBarrelStream;

    TcpMessagingWrapper *consumer;
    TcpMessagingOpts *cfg;

    BaseError currentErr;

    void handleError(const QString &err) override;
};

#endif // GUN_FEEDBACK_BARREL_STREAM_H
