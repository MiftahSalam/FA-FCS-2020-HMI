#ifndef STREAM_DATETIME_H
#define STREAM_DATETIME_H

#include <QObject>
#include <QWidget>

#include "src/domain/osd/repository/datetime_repository.h"
#include "src/infra/messaging/tcp/tcp_messaging_wrapper.h"
#include "src/model/osd/datetime_model.h"
#include "src/shared/config/messaging_tcp_config.h"
#include "src/usecase/osd/stream/IOSDStream.h"

class StreamDateTime : public QObject, public IOSDStream<DateTimeModel>
{
    Q_OBJECT
public:
    StreamDateTime(StreamDateTime &other) = delete;
    void operator=(const StreamDateTime&) = delete;
    static StreamDateTime *getInstance(
        TcpMessagingOpts *config,
        DateTimeRepository *repoDateTime
        );

    BaseError check() override;

signals:
    void signalDataProcessed(DateTimeModel data) override;

protected:
    StreamDateTime(
        TcpMessagingOpts *config = nullptr,
        DateTimeRepository *repoDateTime = nullptr
        );

private slots:
    void onDataReceived(QByteArray data) override;

private:
    static StreamDateTime *dateTimeStream;
    TcpMessagingWrapper *consumer;
    TcpMessagingOpts *cfg;
    DateTimeRepository *_repoDateTime;

    BaseError currentErr;

    void handleError(const QString &err) override;
};

#endif // STREAM_DATETIME_H
