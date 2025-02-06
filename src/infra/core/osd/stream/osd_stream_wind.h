#ifndef OSDSTREAMWIND_H
#define OSDSTREAMWIND_H

#include "src/infra/core/osd/model/wind/wind_stream_model.h"
#include "src/infra/messaging/tcp/tcp_messaging_wrapper.h"
#include "src/infra/messaging/IOSDStream.h"
#include "src/shared/config/messaging_tcp_config.h"

#include <QObject>

class OSDStreamWind : public QObject, public IOSDStream<WindStreamModel>
{
    Q_OBJECT
public:
    OSDStreamWind(OSDStreamWind &other) = delete;
    void operator=(const OSDStreamWind&) = delete;
    static OSDStreamWind* getInstance(
        TcpMessagingOpts *config);

    BaseError check() override;

signals:
    // IOSDStream interface
    void signalDataProcessed(WindStreamModel data) override;

protected:
    OSDStreamWind(
            TcpMessagingOpts *config = nullptr
            );

    // IOSDStream interface
private slots:
    void onDataReceived(QByteArray data) override;

private:
    static OSDStreamWind *windStream;

    TcpMessagingWrapper *consumer;
    TcpMessagingOpts *cfg;

    BaseError currentErr;

    void handleError(const QString &err) override;
};

#endif // OSDSTREAMWIND_H
