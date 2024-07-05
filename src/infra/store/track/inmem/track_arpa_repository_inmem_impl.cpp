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
}

const TrackBaseEntity *TrackArpaRepositoryInMemImpl::FindOne(const int &trackId) const
{
}

std::list<TrackBaseEntity *> TrackArpaRepositoryInMemImpl::FindAll() const
{
}

void TrackArpaRepositoryInMemImpl::Update(const TrackBaseEntity &track)
{
}

void TrackArpaRepositoryInMemImpl::Remove(const int &trackId)
{
}
