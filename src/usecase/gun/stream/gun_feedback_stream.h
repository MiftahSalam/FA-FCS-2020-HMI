#ifndef GUNFEEDBACKSTREAM_H
#define GUNFEEDBACKSTREAM_H

#include "src/domain/gun/repository/gun_feedback_repository.h"
#include "src/infra/messaging/tcp/tcp_messaging_wrapper.h"
#include "src/model/gun/gun_feedback_model.h"
#include "src/shared/config/messaging_tcp_config.h"
#include "src/usecase/gun/stream/gun_stream_base.h"

#include <QObject>

class GunFeedbackStream : public QObject, public GunStreamBase<GunFeedbackModel>
{
    Q_OBJECT
public:
    GunFeedbackStream(GunFeedbackStream &other) = delete;
    void operator = (const GunFeedbackStream&) = delete;
    static GunFeedbackStream *getInstance(
        TcpMessagingOpts *config,
        GunFeedbackRepository *repoGunFback
        );
    // TODO: add members and implementation
protected:
    GunFeedbackStream(
        TcpMessagingOpts *_config = nullptr,
        GunFeedbackRepository *_repoGunFback = nullptr
        );

    // GunStreamBase interface
signals:
    void signalDataProcessed(GunFeedbackModel data) override;

public:
    BaseError check() override;

private slots:
    void onDataReceived(QByteArray data) override;
    void periodUpdate() override;

private:
    void handleError(const QString &err) override;

    static GunFeedbackStream *gunFeedbackStream;
    TcpMessagingWrapper *consumer;
    TcpMessagingOpts *cfg;
    GunFeedbackRepository* repoGunFback;
    BaseError currentErr;

};

#endif // GUNFEEDBACKSTREAM_H
