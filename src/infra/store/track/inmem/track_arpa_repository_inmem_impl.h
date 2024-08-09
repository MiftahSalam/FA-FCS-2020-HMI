#ifndef TRACKARPAREPOSITORYINMEMIMPL_H
#define TRACKARPAREPOSITORYINMEMIMPL_H

#include <QMap>
#include "src/domain/track/repository/track_base_repository.h"
#include "src/infra/store/track/track_repository_listener.h"
#include "src/infra/store/track/track_repository_publisher.h"

class TrackArpaRepositoryInMemImpl: public TrackBaseRepository, public TrackRepositoryPublisher
{
public:
    static TrackBaseRepository *GetInstance();

    // TrackRepositoryPublisher interface
    void AddListener(TrackRepositoryListener *listener) override;
    void RemoveListener(TrackRepositoryListener *listener) override;

    // TrackBaseRepository interface
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

    TrackBaseEntity *_entity;
};

#endif // TRACKARPAREPOSITORYINMEMIMPL_H
