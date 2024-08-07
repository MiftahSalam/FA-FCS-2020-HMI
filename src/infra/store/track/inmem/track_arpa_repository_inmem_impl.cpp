#include "track_arpa_repository_inmem_impl.h"

TrackArpaRepositoryInMemImpl* TrackArpaRepositoryInMemImpl::instance{nullptr};

TrackArpaRepositoryInMemImpl::TrackArpaRepositoryInMemImpl(TrackBaseEntity *entity)
{
    _tracks.clear();
}

TrackBaseRepository *TrackArpaRepositoryInMemImpl::GetInstance()
{
    if (instance == nullptr) {
        TrackBaseEntity *entity = new TrackBaseEntity(0,0,0,0,0,"","");
           instance = new TrackArpaRepositoryInMemImpl(entity);
       }

    return instance;
}

void TrackArpaRepositoryInMemImpl::addListener(TrackRepositoryListener *listener)
{

}

void TrackArpaRepositoryInMemImpl::removeListener(TrackRepositoryListener *listener)
{

}

void TrackArpaRepositoryInMemImpl::Insert(const TrackBaseEntity &track)
{
    //check track exist
    auto it =_tracks.find(track.Id());
    if (it != _tracks.end()) {
        Update(track);// track found
    }else{
        // TODO: insert to repo
        TrackBaseEntity *newTrack = _tracks.value(track.Id(), nullptr);
        newTrack->setRange(track.range());
        newTrack->setBearing(track.bearing());
        newTrack->setSpeed(track.speed());
        newTrack->setCourse(track.course());

        // TODO: update TracksAdded
        QList<TrackBaseEntity> tnList;
        tnList << track;

        //update listener
        foreach (TrackRepositoryListener *listener, listeners) {
            // listener->OnTracksAdded(tnList);
        }
    }

}

const TrackBaseEntity *TrackArpaRepositoryInMemImpl::FindOne(const int &trackId) const
{
}

std::list<TrackBaseEntity *> TrackArpaRepositoryInMemImpl::FindAll() const
{
}

void TrackArpaRepositoryInMemImpl::Update(const TrackBaseEntity &track)
{
    // TODO: insert to repo
    TrackBaseEntity *existingTrack = _tracks.value(track.Id(), nullptr);
    existingTrack->setRange(track.range());
    existingTrack->setBearing(track.bearing());
    existingTrack->setSpeed(track.speed());
    existingTrack->setCourse(track.course());
    // TODO: update listeners TrackPropertyChanged
    foreach (TrackRepositoryListener *listener, listeners) {
        listener->OnTrackPropertyChanged(track.Id(),existingTrack);
    }
}

void TrackArpaRepositoryInMemImpl::Remove(const int &trackId)
{
    // TODO: insert to repo

    // TODO: update listeners TracksRemoved
}
