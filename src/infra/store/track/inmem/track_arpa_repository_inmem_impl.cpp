#include "track_arpa_repository_inmem_impl.h"

TrackArpaRepositoryInMemImpl* TrackArpaRepositoryInMemImpl::instance{nullptr};

TrackArpaRepositoryInMemImpl::TrackArpaRepositoryInMemImpl()
{
    _tracks.clear();
}

TrackBaseRepository *TrackArpaRepositoryInMemImpl::GetInstance()
{
    if (instance == nullptr) {
        // TrackBaseEntity *entity = new TrackBaseEntity(0,0,0,0,0,"","",0);
        instance = new TrackArpaRepositoryInMemImpl();
    }

    return instance;
}

void TrackArpaRepositoryInMemImpl::Insert(const TrackBaseEntity &track)
{
    // insert to repo
    TrackBaseEntity *newTrack = new TrackBaseEntity(
                track.Id(),
                track.range(),
                track.bearing(),
                track.speed(),
                track.course(),
                track.source(),
                track.status(),
                track.timeStamp()
                );

    _tracks.insert(track.Id(), newTrack);

    // update TracksAdded
    QList<TrackBaseEntity*> tnList;
    tnList << newTrack;

    //update listener
    foreach (TrackRepositoryListener *listener, listeners) {
        listener->OnTracksAdded(std::list<TrackBaseEntity*>( tnList.begin(), tnList.end()));
    }
}

const TrackBaseEntity *TrackArpaRepositoryInMemImpl::FindOne(const int &trackId) const
{
    return _tracks.value(trackId);
}

std::list<TrackBaseEntity *> TrackArpaRepositoryInMemImpl::FindAll() const
{
    QList<TrackBaseEntity*> tnList = _tracks.values();
    return std::list<TrackBaseEntity*>(tnList.begin(), tnList.end());
}

void TrackArpaRepositoryInMemImpl::Update(const TrackBaseEntity &track)
{
    // check track exist
    TrackBaseEntity* existingTrack = _tracks.value(track.Id(), nullptr);
    if (existingTrack) {
        // track found
        // update repo
        existingTrack->setRange(track.range());
        existingTrack->setBearing(track.bearing());
        existingTrack->setSpeed(track.speed());
        existingTrack->setCourse(track.course());
        existingTrack->setTimeStamp(track.timeStamp());
        existingTrack->setSource(track.source());
        existingTrack->setStatus(track.status());

        //update listeners TrackPropertyChanged
        foreach (TrackRepositoryListener *listener, listeners) {
            listener->OnTrackPropertyChanged(existingTrack->Id(), existingTrack);
        }
    } else {
        Insert(track);
    }
}

void TrackArpaRepositoryInMemImpl::Remove(const int &trackId)
{
    // remove from repo
    _tracks.take(trackId);

    // update listeners TracksRemoved
    QList<int> tnIdList;
    tnIdList << trackId;

    //update listener
    foreach (TrackRepositoryListener *listener, listeners) {
        listener->OnTracksRemoved(std::list<int>( tnIdList.begin(), tnIdList.end()));
    }
}

void TrackArpaRepositoryInMemImpl::AddListener(TrackRepositoryListener *listener)
{
    listeners.append(listener);
}

void TrackArpaRepositoryInMemImpl::RemoveListener(TrackRepositoryListener *listener)
{
    listeners.removeAll(listener);
}
