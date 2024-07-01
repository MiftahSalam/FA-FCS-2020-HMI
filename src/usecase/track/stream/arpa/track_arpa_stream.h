#ifndef TRACKARPASTREAM_H
#define TRACKARPASTREAM_H

#include "src/domain/track/repository/track_base_repository.h"
#include "src/infra/messaging/tcp/tcp_messaging_wrapper.h"
#include "src/model/track/arpa/track_arpa_model.h"
#include "src/shared/config/messaging_tcp_config.h"
#include "src/usecase/track/stream/track_stream_base.h"
#include <QObject>

class TrackArpaStream : public QObject, public TrackStreamBase<TrackArpaModel>
{
    Q_OBJECT
public:
    TrackArpaStream(TrackArpaStream &other) = delete;
    void operator=(const TrackArpaStream&) = delete;
    static TrackArpaStream* getInstance(TcpMessagingOpts *config, TrackBaseRepository* _repoArpa);

    BaseError check() override;

signals:
    // TrackStreamBase interface
    void signalDataProcessed(TrackArpaModel data) override;

protected:
    TrackArpaStream(TcpMessagingOpts *config = nullptr, TrackBaseRepository* repoArpa = nullptr);

    // TrackStreamBase interface
private slots:
    void onDataReceived(QByteArray data) override;
    void periodUpdate() override;

private:
    static TrackArpaStream *arpaStream;

    TcpMessagingWrapper *consumer;
    TcpMessagingOpts *cfg;
    TrackBaseRepository* _repoArpa;
};
#endif // TRACKARPASTREAM_H
