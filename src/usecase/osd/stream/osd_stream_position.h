#ifndef OSDSTREAMPOSITION_H
#define OSDSTREAMPOSITION_H

#include "src/domain/osd/repository/osd_position_repository.h"
#include "src/infra/messaging/tcp/tcp_messaging_wrapper.h"
#include "src/model/osd/position_model.h"
//#include "src/shared/config/amqp_config.h"
#include "src/shared/config/messaging_tcp_config.h"
#include "src/usecase/osd/cms/osd_cms_input_mode.h"
#include "src/usecase/osd/stream/IOSDStream.h"

#include <QObject>

class OSDStreamPosition : public QObject, public IOSDStream<PositionModel>
{
    Q_OBJECT
public:
    OSDStreamPosition(OSDStreamPosition &other) = delete;
    void operator=(const OSDStreamPosition&) = delete;
    static OSDStreamPosition* getInstance(
            TcpMessagingOpts *config,
            OSDPositionRepository *_repoPos,
            OSDCMSInputMode *modeService
            );
//    static OSDStreamPosition* getInstance(AMQPConfig *config);

    BaseError check() override;

signals:
    // IOSDStream interface
    void signalDataProcessed(PositionModel data) override;

protected:
    OSDStreamPosition(
            TcpMessagingOpts *config = nullptr,
            OSDPositionRepository *repoPos = nullptr,
            OSDCMSInputMode *modeService = nullptr
            );
//    OSDStreamPosition(AMQPConfig *config = nullptr);

    // IOSDStream interface
private slots:
    void onDataReceived(QByteArray data) override;

private:
    static OSDStreamPosition *positionStream;

//    AmqpConsumerWrapper *consumer;
    TcpMessagingWrapper *consumer;
    TcpMessagingOpts *cfg;
    OSDPositionRepository* _repoPos;
    OSDCMSInputMode *serviceMode;
    // TODO: add input mode repo
//    AMQPConfig *cfg;

    BaseError currentErr;

    void handleError(const QString &err) override;
};

#endif // OSDSTREAMPOSITION_H
