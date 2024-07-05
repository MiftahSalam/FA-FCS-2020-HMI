#include "track_repository.h"
#include "src/infra/store/track/inmem/track_arpa_repository_inmem_impl.h"

TrackRepository::TrackRepository(QObject *parent)
    : QObject{parent}
{
    repoTrackArpa = TrackArpaRepositoryInMemImpl::GetInstance();

}

TrackBaseRepository *TrackRepository::getRepoTrackArpa() const
{
    return repoTrackArpa;
}
