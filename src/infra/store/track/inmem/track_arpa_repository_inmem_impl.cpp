#include "track_arpa_repository_inmem_impl.h"

TrackArpaRepositoryInMemImpl* TrackArpaRepositoryInMemImpl::instance{nullptr};

TrackArpaRepositoryInMemImpl::TrackArpaRepositoryInMemImpl()
{
    _tracks.clear();
}

TrackBaseRepository *TrackArpaRepositoryInMemImpl::GetInstance()
{
    if (instance == nullptr) {
        instance = new TrackArpaRepositoryInMemImpl();
    }

    return instance;
}

void TrackArpaRepositoryInMemImpl::Insert(const TrackBaseEntity &track)
{
    // insert to repo
    TrackBaseEntity *newTrack = new TrackBaseEntity(
                track.getId(),
                track.getRange(),
                track.getBearing(),
                track.getSpeed(),
                track.getCourse(),
                track.source(),
                track.status(),
                track.getTimeStamp()
                );

    _tracks.insert(track.getId(), newTrack);

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
    TrackBaseEntity* existingTrack = _tracks.value(track.getId(), nullptr);
    if (existingTrack) {
        // track found
        // update repo
        existingTrack->setRange(track.getRange());
        existingTrack->setBearing(track.getBearing());
        existingTrack->setSpeed(track.getSpeed());
        existingTrack->setCourse(track.getCourse());
        existingTrack->setTimeStamp(track.getTimeStamp());
        existingTrack->setSource(track.source());
        existingTrack->setStatus(track.status());

        _tracks.insert(track.getId(), existingTrack);

        //update listeners TrackPropertyChanged
        foreach (TrackRepositoryListener *listener, listeners) {
            listener->OnTrackPropertyChanged(existingTrack->getId(), existingTrack);
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
