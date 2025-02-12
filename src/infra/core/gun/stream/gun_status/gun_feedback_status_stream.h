#ifndef GUNFEEDBACKSTREAM_H
#define GUNFEEDBACKSTREAM_H

#include "src/infra/messaging/IOSDStream.h"
#include "src/infra/messaging/tcp/tcp_messaging_wrapper.h"
#include "src/infra/core/gun/stream/gun_status/gun_feedback_status_model.h"
#include "src/shared/config/messaging_tcp_config.h"

#include <QObject>

class GunFeedbackStatusStream : public QObject, public IOSDStream<GunFeedbackStatusModel>
{
    Q_OBJECT
public:
    GunFeedbackStatusStream(GunFeedbackStatusStream &other) = delete;
    void operator = (const GunFeedbackStatusStream&) = delete;
    static GunFeedbackStatusStream *getInstance(
        TcpMessagingOpts *config
        );

    BaseError check() override;

signals:
    void signalDataProcessed(GunFeedbackStatusModel data) override;

protected:
    GunFeedbackStatusStream(
        TcpMessagingOpts *_config = nullptr
        );

private slots:
    void onDataReceived(QByteArray data) override;

private:    
    static GunFeedbackStatusStream *gunFeedbackStream;

    TcpMessagingWrapper *consumer;
    TcpMessagingOpts *cfg;

    BaseError currentErr;

    void handleError(const QString &err) override;
};

#endif // GUNFEEDBACKSTREAM_H
