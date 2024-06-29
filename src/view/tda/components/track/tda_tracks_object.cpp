#include "tda_tracks_object.h"

TDATracksObject::TDATracksObject(QObject *parent)
    : TDAZoomableObjectBase{parent}
{

}


void TDATracksObject::Draw(QPainter *painter, const int &side, const int &width, const int &height, const QPoint &off_center)
{
}

void TDATracksObject::OnZoom(float range)
{
}
