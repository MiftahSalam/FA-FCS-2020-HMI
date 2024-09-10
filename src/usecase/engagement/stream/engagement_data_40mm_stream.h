#ifndef ENGAGEMENTDATA40MMSTREAM_H
#define ENGAGEMENTDATA40MMSTREAM_H

#include "src/domain/engagement/repository/engagement_data_repository.h"
#include "src/infra/messaging/tcp/tcp_messaging_wrapper.h"
#include "src/model/engagement/engagement_data_model.h"
#include "src/shared/config/messaging_tcp_config.h"
#include "src/usecase/engagement/stream/engagement_data_base_stream.h"
#include <QObject>
#include <QTimer>

class EngagementData40mmStream : public QObject, public EngagementStreamBase<EngagementDataModel>
{
    Q_OBJECT
public:
    EngagementData40mmStream(EngagementData40mmStream &other) = delete;
    void operator=(const EngagementData40mmStream&) = delete;
    static EngagementData40mmStream* getInstance(
            TcpMessagingOpts *config,
            EngagementDataRepository* repoEngagemenData);

    void CreateEngage(const int &trackId);
    void DeleteEngage();

    // EngagementStreamBase interface
    BaseError check() override;

    // EngagementStreamBase interface
signals:
    void signalDataProcessed(EngagementDataModel data) override;

    // EngagementStreamBase interface
private slots:
    void onDataReceived(QByteArray data) override;
    void periodeUpdate();

protected:
    EngagementData40mmStream(
            TcpMessagingOpts *config,
            EngagementDataRepository* _repoEngagemenData
            );

    QTimer *timer;

private:
    static EngagementData40mmStream *instance;

    TcpMessagingWrapper *consumer;
    TcpMessagingOpts *cfg;
    EngagementDataRepository* _repoEngagemenData;

    BaseError currentErr;
    int currentTrack;

};

#endif // ENGAGEMENTDATA40MMSTREAM_H
