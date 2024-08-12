#include "tda_tracks_object.h"
#include "src/infra/store/track/track_repository_publisher.h"

#include <QDebug>

TDATracksObject::TDATracksObject(QObject *parent, TrackBaseRepository *repoTrack)
    : TDAZoomableObjectBase{parent}, arpaRepo(repoTrack)
{
    // TODO: add this to listener
    TrackRepositoryPublisher *publisher = dynamic_cast<TrackRepositoryPublisher*>(arpaRepo);
    publisher->AddListener(this);
}


void TDATracksObject::Draw(QPainter *painter, const int &side, const int &width, const int &height, const QPoint &off_center)
{
}

void TDATracksObject::OnZoom(float range)
{
}


void TDATracksObject::OnTracksAdded(std::list<TrackBaseEntity *> tnList)
{
    qDebug()<<Q_FUNC_INFO<<"tnList size"<<tnList.size();
}

void TDATracksObject::OnTracksRemoved(std::list<int> tnIdList)
{
}

void TDATracksObject::OnTrackPropertyChanged(int tn, TrackBaseEntity *track)
{
}
