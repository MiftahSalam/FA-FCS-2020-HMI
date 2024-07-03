#ifndef OSD_STREAM_GYRO_H
#define OSD_STREAM_GYRO_H

#include <QObject>
#include <QWidget>

#include "src/domain/osd/repository/osd_inertia_repository.h"
#include "src/infra/messaging/tcp/tcp_messaging_wrapper.h"
#include "src/model/osd/gyro_model.h"
#include "src/shared/config/messaging_tcp_config.h"
#include "src/usecase/osd/stream/IOSDStream.h"

class OSDStreamGyro : public QObject, public IOSDStream<GyroModel>
{
    Q_OBJECT
public:
    OSDStreamGyro(OSDStreamGyro &other) = delete;
    void operator=(const OSDStreamGyro&) = delete;
    static OSDStreamGyro* getInstance(TcpMessagingOpts *config, OSDInertiaRepository *repoInertia);

    BaseError check() override;

signals:
    void signalDataProcessed(GyroModel data) override;

protected:
    OSDStreamGyro(TcpMessagingOpts *config = nullptr, OSDInertiaRepository *repoInertia = nullptr);

private slots:
    void onDataReceived(QByteArray data) override;

private:
    static OSDStreamGyro *gyroStream;

    TcpMessagingWrapper *consumer;
    TcpMessagingOpts *cfg;
    OSDInertiaRepository* _repoInertia;
};

#endif // OSD_STREAM_GYRO_H
