#ifndef TDATRACKSOBJECT_H
#define TDATRACKSOBJECT_H

#include "src/view/tda/components/tda_object_base.h"
#include "src/infra/store/track/track_repository_listener.h"

//TODO: TrackRepo listener inherit
class TDATracksObject : public TDAZoomableObjectBase, public TrackRepositoryListener
{
    Q_OBJECT
public:
    explicit TDATracksObject(QObject *parent = nullptr);

    // TDAObjectBase interface
    void Draw(QPainter *painter, const int &side, const int &width, const int &height, const QPoint &off_center) override;

    //TODO: implement track changed handler
    // TrackRepositoryListener interface
public:
    void OnTracksAdded(std::list<TrackBaseEntity *> tnList) override;
    void OnTracksRemoved(std::list<TrackBaseEntity *> tnList) override;
    void OnTrackPropertyChanged(int tn, TrackBaseEntity *track) override;

    // TDAZoomableObjectBase interface
public slots:
    void OnZoom(float range) override;

private:
    //TODO: add trackRepo member

};

#endif // TDATRACKSOBJECT_H
