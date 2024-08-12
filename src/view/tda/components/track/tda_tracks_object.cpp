#include "tda_tracks_object.h"
#include "src/infra/store/track/track_repository_publisher.h"

#include <QDebug>

TDATracksObject::TDATracksObject(QObject *parent, TrackBaseRepository *repoTrack)
    : TDAZoomableObjectBase{parent}, arpaRepo(repoTrack)
{
    //add this to listener
    TrackRepositoryPublisher *publisher = dynamic_cast<TrackRepositoryPublisher*>(arpaRepo);
    publisher->AddListener(this);
    /*
    */
    Track* tr = new Track(dynamic_cast<QWidget*>(parent), QSize(60,20));
    tr->buildUI({});
    tr->setGeometry(400,400, 60,20);
    tr->adjustSize();
    tr->show();
    trackObjListMap.insert(1, tr);
}

TDATracksObject::~TDATracksObject()
{
    qDebug()<<Q_FUNC_INFO;
    trackObjListMap.clear();
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
    qDebug()<<Q_FUNC_INFO<<"tnIdList size"<<tnIdList.size();
}

void TDATracksObject::OnTrackPropertyChanged(int tn, TrackBaseEntity *track)
{
}
