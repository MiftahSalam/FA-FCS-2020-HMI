#ifndef TDATRACKSOBJECT_H
#define TDATRACKSOBJECT_H

#include "src/domain/track/repository/track_base_repository.h"
#include "src/view/tda/components/tda_object_base.h"
#include "src/infra/store/track/track_repository_listener.h"
#include "src/view/tda/components/track/track.h"

// TODO: TrackRepo listener inherit
class TDATracksObject : public TDAZoomableObjectBase, public TrackRepositoryListener
{
    Q_OBJECT
public:
    explicit TDATracksObject(QObject *parent = nullptr,
            TrackBaseRepository *repoTrack = nullptr,
            double scale = 0.
            );

    ~TDATracksObject();

    // TDAObjectBase interface
    void Draw(QPainter *painter, const int &side, const int &width, const int &height, const QPoint &off_center) override;

    // TODO: implement track changed handler
    //  TrackRepositoryListener interface
    void OnTracksAdded(std::list<TrackBaseEntity *> tnList) override;
    void OnTracksRemoved(std::list<int> tnIdList) override;
    void OnTrackPropertyChanged(int tn, TrackBaseEntity *track) override;

    // TDAZoomableObjectBase interface
public slots:
    void OnZoom(float range) override;

private:
    // add trackRepo member
    TrackBaseRepository *arpaRepo;

    // TODO: add tracksList member
    QMap<int, Track*> trackObjListMap;

    QWidget* parentWidget;

    double tdaScale;

    void generateTrackUI(TrackBaseEntity *newTrack);
    TrackParam entityToTrackParam(TrackBaseEntity *track);
    int range2Pixel(double range);
    QPoint polar2Cartesia(double range, double bearing);
};

#endif // TDATRACKSOBJECT_H
