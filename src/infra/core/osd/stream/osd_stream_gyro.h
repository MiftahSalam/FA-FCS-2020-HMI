#ifndef OSD_STREAM_GYRO_H
#define OSD_STREAM_GYRO_H

#include <QObject>
#include <QWidget>

#include "src/infra/core/osd/model/inertia/gyro_stream_model.h"
#include "src/infra/messaging/tcp/tcp_messaging_wrapper.h"
#include "src/infra/messaging/IOSDStream.h"
#include "src/shared/config/messaging_tcp_config.h"

class OSDStreamGyro : public QObject, public IOSDStream<GyroStreamModel>
{
    Q_OBJECT
public:
    OSDStreamGyro(OSDStreamGyro &other) = delete;
    void operator=(const OSDStreamGyro&) = delete;
    static OSDStreamGyro* getInstance(
            TcpMessagingOpts *config
            );

    BaseError check() override;

signals:
    void signalDataProcessed(GyroStreamModel data) override;

protected:
    OSDStreamGyro(
            TcpMessagingOpts *config = nullptr
            );

private slots:
    void onDataReceived(QByteArray data) override;

private:
    static OSDStreamGyro *gyroStream;

    TcpMessagingWrapper *consumer;
    TcpMessagingOpts *cfg;

    BaseError currentErr;

    void handleError(const QString &err) override;
};

#endif // OSD_STREAM_GYRO_H
