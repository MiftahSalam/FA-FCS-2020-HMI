#ifndef TRACK_STREAM_H
#define TRACK_STREAM_H

#include <QObject>

#include "src/infra/store/track/track_repository.h"
#include "src/shared/config/arpa_config.h"
#include "src/shared/config/messaging_tcp_config.h"
#include "src/usecase/track/stream/arpa/track_arpa_stream.h"


class TrackStream :public QObject
{
    Q_OBJECT
public:
    TrackStream(
            QObject *parent = nullptr,
            MessagingTcpConfig *config = nullptr,
            ArpaConfig *confiArpa = nullptr,
            TrackRepository *repoTrack = nullptr
            );

    TrackArpaStream *getServiceTrackArpaStream() const;

private:
    MessagingTcpConfig *consumerConfig;
    ArpaConfig *arpaConfig;
    TrackArpaStream *serviceTrackArpaStream;
};

#endif // TRACK_STREAM_H
