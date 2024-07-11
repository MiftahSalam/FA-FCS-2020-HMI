#ifndef TDATRACKSOBJECT_H
#define TDATRACKSOBJECT_H

#include "src/view/tda/components/tda_object_base.h"

//TODO: TrackRepo listener inherit
class TDATracksObject : public TDAZoomableObjectBase
{
    Q_OBJECT
public:
    explicit TDATracksObject(QObject *parent = nullptr);

signals:


    // TDAObjectBase interface
public:
    void Draw(QPainter *painter, const int &side, const int &width, const int &height, const QPoint &off_center) override;

    // TDAZoomableObjectBase interface
public slots:
    void OnZoom(float range) override;

    //TODO: implement track changed handler
//    void OnTracksRemoved(QList<TrackDomain*> trList);
    //    void OnTracksAdded(QList<TrackDomain*> trList);
    //    void OnTrackPropertiesChanged(int tn, TrackProps props);

private:
    //TODO: add trackRepo member

    //TODO: add tracksList member

    //TODO: add track ui generator method
};

#endif // TDATRACKSOBJECT_H
