#ifndef TRACK_REPOSITORY_LISTENER_H
#define TRACK_REPOSITORY_LISTENER_H

#include <list>
#include "src/domain/track/entity/track_base_entity.h"

class TrackRepositoryListener
{
public:
    virtual void OnTracksAdded(std::list<TrackBaseEntity*> tnList) = 0;
    virtual void OnTracksRemoved(std::list<int> tnIdList) = 0;
    virtual void OnTrackPropertyChanged(int tn, TrackBaseEntity* track) = 0;
};

#endif // TRACK_REPOSITORY_LISTENER_H
