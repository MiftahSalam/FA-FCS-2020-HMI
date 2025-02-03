#ifndef OSD_STREAM_DATETIME_H
#define OSD_STREAM_DATETIME_H

#include <QObject>
#include <QWidget>
#include <QTimer>

#include "src/infra/core/osd/model/date_time/date_time_stream_model.h"
#include "src/infra/messaging/tcp/tcp_messaging_wrapper.h"
#include "src/infra/messaging/IOSDStream.h"
#include "src/shared/config/messaging_tcp_config.h"

class OSDStreamDateTime : public QObject, public IOSDStream<DateTimeStreamModel>
{
    Q_OBJECT
public:
    OSDStreamDateTime(OSDStreamDateTime &other) = delete;
    void operator=(const OSDStreamDateTime&) = delete;
    static OSDStreamDateTime *getInstance(
        TcpMessagingOpts *config
        );

    BaseError check() override;

signals:
    void signalDataProcessed(DateTimeStreamModel data) override;

protected:
    OSDStreamDateTime(
        TcpMessagingOpts *config = nullptr
        );

private slots:
    void onDataReceived(QByteArray data) override;

private:
    static OSDStreamDateTime *dateTimeStream;

    TcpMessagingWrapper *consumer;
    TcpMessagingOpts *cfg;

    BaseError currentErr;

    void handleError(const QString &err) override;
};

#endif // OSD_STREAM_DATETIME_H
