#ifndef TRACK_REPOSITORY_PUBLISHER_H
#define TRACK_REPOSITORY_PUBLISHER_H

#include "src/infra/store/track/track_repository_listener.h"

class TrackRepositoryPublisher
{
public:
    virtual void AddListener(TrackRepositoryListener *listener) = 0;
    virtual void RemoveListener(TrackRepositoryListener *listener) = 0;
    virtual std::list<TrackRepositoryListener*> GetListeners() = 0;
};

#endif // TRACK_REPOSITORY_PUBLISHER_H
