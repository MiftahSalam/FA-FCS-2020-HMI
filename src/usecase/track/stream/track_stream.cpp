#include "track_stream.h"
#include "src/shared/common/errors/err_object_creation.h"

TrackStream::TrackStream(
        QObject *parent,
        MessagingTcpConfig *config,
        ArpaConfig *confiArpa,
        TrackRepository *repoTrack
        ): QObject (parent), consumerConfig(config)
{
    if (config == nullptr)
    {
        throw ErrObjectCreation();
    }

    if (confiArpa == nullptr)
    {
        throw ErrObjectCreation();
    }

    if (repoTrack == nullptr)
    {
        throw ErrObjectCreation();
    }

    TcpMessagingOpts *arpaTrackStreamVal = config->getInstance("")->getContent().value("arpa");
    serviceTrackArpaStream = TrackArpaStream::getInstance(
                arpaTrackStreamVal,
                confiArpa,
                repoTrack->getRepoTrackArpa()
                );
}

TrackArpaStream *TrackStream::getServiceTrackArpaStream() const
{
    return serviceTrackArpaStream;
}
