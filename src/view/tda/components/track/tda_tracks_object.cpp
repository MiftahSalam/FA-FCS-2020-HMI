#include "tda_tracks_object.h"
#include "qmath.h"
#include "src/infra/store/track/track_repository_publisher.h"
#include "src/shared/common/errors/err_object_creation.h"
#include "src/shared/utils/utils.h"

#include <QDebug>
#include <cmath>

TDATracksObject::TDATracksObject(QObject *parent, TrackBaseRepository *repoTrack, double scale)
    : TDAZoomableObjectBase{parent}, arpaRepo(repoTrack), tdaScale(scale)
{
    parentWidget = dynamic_cast<QWidget *>(parent);
    if (parentWidget == nullptr)
    {
        throw ErrObjectCreation();
    }

    // add this to listener
    TrackRepositoryPublisher *publisher = dynamic_cast<TrackRepositoryPublisher *>(arpaRepo);
    publisher->AddListener(this);
}

TDATracksObject::~TDATracksObject()
{
    qDebug() << Q_FUNC_INFO;
    trackObjListMap.clear();
}

void TDATracksObject::Draw(QPainter *painter, const int &side, const int &width, const int &height, const QPoint &off_center)
{
    Q_UNUSED(painter);
    Q_UNUSED(side);
    Q_UNUSED(width);
    Q_UNUSED(height);
    Q_UNUSED(off_center);
}

void TDATracksObject::OnZoom(float range)
{
    tdaScale = range;
    foreach (auto tr, trackObjListMap)
    {
        tr->move(polar2Cartesian(tr->getTrackData()->getRange(), tr->getTrackData()->getBearing()));
    }
}

void TDATracksObject::OnIdentityChange(int tn, TrackUtils::Identity newIdentity)
{
    const TrackBaseEntity *findTrack = arpaRepo->FindOne(tn);
    if (findTrack)
    {
        TrackBaseEntity updateTrack(
            findTrack->getId(),
            findTrack->getRange(),
            findTrack->getBearing(),
            findTrack->getSpeed(),
            findTrack->getCourse(),
            findTrack->source(),
            findTrack->status(),
            findTrack->getTimeStamp());
        updateTrack.setCurrIdentity(newIdentity);
        updateTrack.setCurrEnv(findTrack->getCurrEnv());
        updateTrack.setCurrSource(findTrack->getCurrSource());

        arpaRepo->Update(updateTrack);
    }
}

void TDATracksObject::OnTracksAdded(std::list<TrackBaseEntity *> tnList)
{
    qDebug() << Q_FUNC_INFO << "tnList size" << tnList.size();
    foreach (auto track, tnList)
    {
        generateTrackUI(track);
    }
}

void TDATracksObject::OnTracksRemoved(std::list<int> tnIdList)
{
    qDebug() << Q_FUNC_INFO << "tnIdList size" << tnIdList.size();
    foreach (auto trackId, tnIdList)
    {
        delete trackObjListMap.take(trackId);
    }
}

void TDATracksObject::OnTrackPropertyChanged(int tn, TrackBaseEntity *track)
{
    TdaTrack *findTrack = trackObjListMap.value(tn, nullptr);
    if (findTrack)
    {
        findTrack->updateTrackData(*entityToTrackParam(track));
        findTrack->move(polar2Cartesian(findTrack->getTrackData()->getRange(), findTrack->getTrackData()->getBearing()));
    }
}
void TDATracksObject::generateTrackUI(TrackBaseEntity *newTrack)
{
    TdaTrack *tr = new TdaTrack(parentWidget, TRACK_WIDGET_SIZE);
    connect(tr, &TdaTrack::identityChange_Signal, this, &TDATracksObject::OnIdentityChange);

    tr->buildUI(entityToTrackParam(newTrack));
    tr->move(polar2Cartesian(newTrack->getRange(), newTrack->getBearing()));
    tr->adjustSize();
    tr->show();
    trackObjListMap.insert(newTrack->getId(), tr);
}

TrackParam *TDATracksObject::entityToTrackParam(TrackBaseEntity *track)
{
    TrackParam *param = new TrackParam(
        track->getId(),
        track->getRange(),
        track->getBearing(),
        0,
        track->getSpeed(),
        track->getCourse(),
        track->getCurrIdentity(),
        track->getCurrSource(),
        track->getCurrEnv(),
        QString::fromStdString(track->getWeaponAssign()));

    return param;
}

QPoint TDATracksObject::polar2Cartesian(double range, double bearing)
{
    QPoint os_pos(parentWidget->width() / 2, parentWidget->height() / 2);
    int rangePixel = Utils::range2Pixel(range, tdaScale, parentWidget->width(), parentWidget->height());
    const double rad2deg = (bearing - 90) * M_PI / 180.;
    int range_pixel_x = rangePixel * qCos(rad2deg) + os_pos.x();
    int range_pixel_y = rangePixel * qSin(rad2deg) + os_pos.y();

    //    qDebug()<<Q_FUNC_INFO<<"os_pos"<<os_pos<<"rangePixel"<<rangePixel<<"bearing"<<bearing<<"rad2deg"<<rad2deg<<"range_pixel_x"<<range_pixel_x<<"range_pixel_y"<<range_pixel_y;

    return QPoint(range_pixel_x-TRACK_ICON_MARGIN.width(), range_pixel_y-TRACK_ICON_MARGIN.height());
}
