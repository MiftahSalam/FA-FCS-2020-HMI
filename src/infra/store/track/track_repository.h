#ifndef TRACKREPOSITORY_H
#define TRACKREPOSITORY_H

#include "src/domain/track/repository/track_base_repository.h"
#include <QObject>

class TrackRepository : public QObject
{
    Q_OBJECT
public:
    explicit TrackRepository(QObject *parent = nullptr);

    TrackBaseRepository *getRepoTrackArpa() const;

private:
    TrackBaseRepository *repoTrackArpa;
};

#endif // TRACKREPOSITORY_H
