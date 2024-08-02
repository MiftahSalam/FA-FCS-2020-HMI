#ifndef TRACKARPAREPOSITORYINMEMIMPL_H
#define TRACKARPAREPOSITORYINMEMIMPL_H

#include <QMap>
#include "src/domain/track/repository/track_base_repository.h"
#include "src/infra/store/track/track_repository_listener.h"

class TrackArpaRepositoryInMemImpl: public TrackBaseRepository
{
public:
    static TrackBaseRepository *GetInstance();

    void addListener(TrackRepositoryListener *listener);
    void removeListener(TrackRepositoryListener *listener);

    // TrackBaseRepository interface
public:
    void Insert(const TrackBaseEntity &track) override;
    const TrackBaseEntity *FindOne(const int &trackId) const override;
    std::list<TrackBaseEntity *> FindAll() const override;
    void Update(const TrackBaseEntity &track) override;
    void Remove(const int &trackId) override;

protected:
    TrackArpaRepositoryInMemImpl();

private:
    static TrackArpaRepositoryInMemImpl *instance;

    QMap<int, TrackBaseEntity*> _tracks;

    QList<TrackRepositoryListener *>listeners;
};

#endif // TRACKARPAREPOSITORYINMEMIMPL_H
