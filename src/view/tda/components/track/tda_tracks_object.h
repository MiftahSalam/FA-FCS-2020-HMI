#ifndef TDATRACKSOBJECT_H
#define TDATRACKSOBJECT_H

#include "src/domain/track/repository/track_base_repository.h"
#include "src/view/tda/components/tda_object_base.h"
#include "src/infra/store/track/track_repository_listener.h"
#include "src/view/tda/components/track/tda_track.h"

class TDATracksObject : public TDAZoomableObjectBase, public TrackRepositoryListener
{
    Q_OBJECT
public:
    explicit TDATracksObject(QObject *parent = nullptr,
                             TrackBaseRepository *repoTrack = nullptr,
                             double scale = 0.);

    ~TDATracksObject();

    // TDAObjectBase interface
    void Draw(QPainter *painter, const int &side, const int &width, const int &height, const QPoint &off_center) override;

    //  TrackRepositoryListener interface
    void OnTracksAdded(std::list<TrackBaseEntity *> tnList) override;
    void OnTracksRemoved(std::list<int> tnIdList) override;
    void OnTrackPropertyChanged(int tn, TrackBaseEntity *track) override;

public slots:
    // TDAZoomableObjectBase interface
    void OnZoom(float range) override;

    void OnIdentityChange(int tn, TrackUtils::Identity newIdentity);

private:
    // add trackRepo member
    TrackBaseRepository *arpaRepo;

    QMap<int, TdaTrack *> trackObjListMap;

    QWidget *parentWidget;

    double tdaScale;

    void generateTrackUI(TrackBaseEntity *newTrack);
    TrackParam *entityToTrackParam(TrackBaseEntity *track);
};

#endif // TDATRACKSOBJECT_H
