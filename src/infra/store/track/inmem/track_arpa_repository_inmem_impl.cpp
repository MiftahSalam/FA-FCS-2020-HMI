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

void TrackArpaRepositoryInMemImpl::addListener(TrackRepositoryListener *listener)
{

}

void TrackArpaRepositoryInMemImpl::removeListener(TrackRepositoryListener *listener)
{

}

void TrackArpaRepositoryInMemImpl::Insert(const TrackBaseEntity &track)
{
    // TODO: insert to repo

    // TODO: update listeners TracksAdded
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

    // TODO: update listeners TrackPropertyChanged
}

void TrackArpaRepositoryInMemImpl::Remove(const int &trackId)
{
    // TODO: insert to repo

    // TODO: update listeners TracksRemoved
}
