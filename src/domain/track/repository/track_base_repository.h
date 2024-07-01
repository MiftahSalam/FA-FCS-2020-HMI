#ifndef TRACK_BASE_REPOSITORY_H
#define TRACK_BASE_REPOSITORY_H

#include "src/domain/track/entity/track_base_entity.h"
#include <list>

class TrackBaseRepository
{
public:
    virtual void Insert(const TrackBaseEntity &track) = 0;
    virtual const TrackBaseEntity* FindOne(const int& trackId) const = 0;
    virtual std::list<TrackBaseEntity*> FindAll() const = 0;
    virtual void Update(const TrackBaseEntity& track) = 0;
    virtual void Remove(const int& trackId) = 0;
};

#endif // TRACK_BASE_REPOSITORY_H
