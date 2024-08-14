#include "tda_tracks_object.h"

TDATracksObject::TDATracksObject(QObject *parent)
    : TDAZoomableObjectBase{parent}
{
    // TODO: add this to listener
}


void TDATracksObject::Draw(QPainter *painter, const int &side, const int &width, const int &height, const QPoint &off_center)
{
}

void TDATracksObject::OnZoom(float range)
{
}


void TDATracksObject::OnTracksAdded(std::list<TrackBaseEntity *> tnList)
{
}

// void TDATracksObject::OnTracksRemoved(std::list<TrackBaseEntity *> tnList)
// {
// }

void TDATracksObject::OnTrackPropertyChanged(int tn, TrackBaseEntity *track)
{
}
