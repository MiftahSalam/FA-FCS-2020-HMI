#include "tda_tracks_object.h"
#include "qmath.h"
#include "src/infra/store/track/track_repository_publisher.h"
#include "src/shared/common/errors/err_object_creation.h"

#include <QDebug>
#include <cmath>

TDATracksObject::TDATracksObject(QObject *parent, TrackBaseRepository *repoTrack, double scale)
    : TDAZoomableObjectBase{parent}, arpaRepo(repoTrack), tdaScale(scale)
{
    parentWidget = dynamic_cast<QWidget*>(parent);
    if (parentWidget == nullptr) {
        throw ErrObjectCreation();
    }

    //add this to listener
    TrackRepositoryPublisher *publisher = dynamic_cast<TrackRepositoryPublisher*>(arpaRepo);
    publisher->AddListener(this);
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
    tdaScale = range;
}

void TDATracksObject::generateTrackUI(TrackBaseEntity *newTrack)
{
    Track* tr = new Track(parentWidget, QSize(60,20));
    tr->buildUI(entityToTrackParam(newTrack));
//    tr->setGeometry(400,400, 60,20);
//    tr->move(400,400);
    tr->move(polar2Cartesia(newTrack->getRange(), newTrack->getBearing()));
    tr->adjustSize();
    tr->show();
    trackObjListMap.insert(newTrack->getId(), tr);
}

TrackParam* TDATracksObject::entityToTrackParam(TrackBaseEntity *track)
{
    TrackParam* param = new TrackParam(
                track->getId(),
                track->getRange(),
                track->getBearing(),
                0,
                track->getSpeed(),
                track->getCourse(),
                track->getCurridentity(),
                track->getCur_source(),
                track->getCurrEnv(),
                QString::fromStdString(track->getWeaponAssign())
                );


    return param;
}

int TDATracksObject::range2Pixel(double range)
{
    int side = qMin(parentWidget->width(), parentWidget->height()) / 2;
    return static_cast<int>(range*(side/(tdaScale)));
}

QPoint TDATracksObject::polar2Cartesia(double range, double bearing)
{
    QPoint os_pos(parentWidget->width()/2,parentWidget->height()/2);
    int rangePixel = range2Pixel(range);
    const double rad2deg = (bearing-90)*M_PI/180.;
    int range_pixel_x = rangePixel*qCos(rad2deg)+os_pos.x();
    int range_pixel_y = rangePixel*qSin(rad2deg)+os_pos.y();

    qDebug()<<Q_FUNC_INFO<<"os_pos"<<os_pos<<"rangePixel"<<rangePixel<<"bearing"<<bearing<<"rad2deg"<<rad2deg<<"range_pixel_x"<<range_pixel_x<<"range_pixel_y"<<range_pixel_y;

    return QPoint(range_pixel_x, range_pixel_y);
//    double range_pixel_x = os_pos.x()-event->pos().x();
//    double range_pixel_y = os_pos.y()-event->pos().y();
//    double bearing = atan2(range_pixel_y,range_pixel_x);
//    bearing = (bearing*180/M_PI)-90;
//    if(bearing<0)
//        bearing+=360;

//    double range = sqrt(pow(range_pixel_y,2)+pow(range_pixel_x,2)); //pixel
//    range = pixel2Range(range); //NM

//    statusBarMousePolar->showMessage(QString("Range : %1, Bearing : %2").arg(QString::number(range,'f',1)).arg(QString::number(bearing,'f',1)),2000);
//    statusBarMousePolar->setGeometry(10,height()-45,200,20);
}


void TDATracksObject::OnTracksAdded(std::list<TrackBaseEntity *> tnList)
{
    qDebug()<<Q_FUNC_INFO<<"tnList size"<<tnList.size();
    foreach (auto track, tnList) {
        generateTrackUI(track);
    }
}

void TDATracksObject::OnTracksRemoved(std::list<int> tnIdList)
{
    qDebug()<<Q_FUNC_INFO<<"tnIdList size"<<tnIdList.size();
    foreach (auto trackId, tnIdList) {
        delete trackObjListMap.take(trackId);
    }
}

void TDATracksObject::OnTrackPropertyChanged(int tn, TrackBaseEntity *track)
{
}
