#ifndef OSD_STREAM_DATETIME_H
#define OSD_STREAM_DATETIME_H

#include <QObject>
#include <QWidget>
#include <QTimer>

#include "src/domain/osd/repository/osd_datetime_repository.h"
#include "src/infra/messaging/tcp/tcp_messaging_wrapper.h"
#include "src/model/osd/datetime_model.h"
#include "src/shared/config/messaging_tcp_config.h"
#include "src/usecase/osd/stream/IOSDStream.h"

class OSDStreamDateTime : public QObject, public IOSDStream<DateTimeModel>
{
    Q_OBJECT
public:
    OSDStreamDateTime(OSDStreamDateTime &other) = delete;
    void operator=(const OSDStreamDateTime&) = delete;
    static OSDStreamDateTime *getInstance(
        TcpMessagingOpts *config,
        OSDDateTimeRepository *repoDateTime
        );

    BaseError check() override;

signals:
    void signalDataProcessed(DateTimeModel data) override;

protected:
    OSDStreamDateTime(
        TcpMessagingOpts *config = nullptr,
        OSDDateTimeRepository *repoDateTime = nullptr
        );

    QTimer *timer;

private slots:
    void onDataReceived(QByteArray data) override;
    void periodeUpdate();

private:
    static OSDStreamDateTime *dateTimeStream;
    TcpMessagingWrapper *consumer;
    TcpMessagingOpts *cfg;
    OSDDateTimeRepository *_repoDateTime;

    BaseError currentErr;

    void handleError(const QString &err) override;
};

#endif // OSD_STREAM_DATETIME_H
