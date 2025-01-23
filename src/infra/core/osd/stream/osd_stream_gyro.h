#ifndef OSD_STREAM_GYRO_H
#define OSD_STREAM_GYRO_H

#include <QObject>
#include <QWidget>

#include "src/domain/osd/repository/osd_inertia_repository.h"
#include "src/infra/core/osd/cms/input_mode/osd_cms_input_mode.h"
#include "src/infra/core/osd/model/inertia/gyro_model.h"
#include "src/infra/messaging/tcp/tcp_messaging_wrapper.h"
#include "src/infra/messaging/IOSDStream.h"
#include "src/shared/config/messaging_tcp_config.h"

class OSDStreamGyro : public QObject, public IOSDStream<GyroModel>
{
    Q_OBJECT
public:
    OSDStreamGyro(OSDStreamGyro &other) = delete;
    void operator=(const OSDStreamGyro&) = delete;
    static OSDStreamGyro* getInstance(
            TcpMessagingOpts *config,
            OSDInertiaRepository *repoInertia,
            OSDCMSInputMode *modeService
            );

    BaseError check() override;

signals:
    void signalDataProcessed(GyroModel data) override;

protected:
    OSDStreamGyro(
            TcpMessagingOpts *config = nullptr,
            OSDInertiaRepository *repoInertia = nullptr,
            OSDCMSInputMode *modeService = nullptr
            );

private slots:
    void onDataReceived(QByteArray data) override;

private:
    static OSDStreamGyro *gyroStream;

    TcpMessagingWrapper *consumer;
    TcpMessagingOpts *cfg;
    OSDInertiaRepository* _repoInertia;
    OSDCMSInputMode *serviceMode;

    BaseError currentErr;

    void handleError(const QString &err) override;
};

#endif // OSD_STREAM_GYRO_H
