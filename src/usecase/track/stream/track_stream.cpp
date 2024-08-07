#include "track_stream.h"
#include "src/shared/common/errors/err_object_creation.h"

TrackStream::TrackStream(QObject *parent, MessagingTcpConfig *config, TrackRepository *repoTrack):
    QObject (parent), consumerConfig(config)
{
    if (config == nullptr)
    {
        throw ErrObjectCreation();
    }

    if (repoTrack == nullptr)
    {
        throw ErrObjectCreation();
    }

    TcpMessagingOpts *arpaTrackStreamVal = config->getInstance("")->getContent().value("track");
    serviceTrackArpaStream = TrackArpaStream::getInstance(
        arpaTrackStreamVal,
        repoTrack->getRepoTrackArpa()
        );
}

TrackArpaStream *TrackStream::getServiceTrackArpaStream() const
{
    return serviceTrackArpaStream;
}
