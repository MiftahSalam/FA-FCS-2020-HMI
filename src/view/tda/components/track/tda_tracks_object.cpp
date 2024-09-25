#include "tda_tracks_object.h"
#include "src/infra/store/track/track_repository_publisher.h"
#include "src/shared/common/errors/err_object_creation.h"
#include "src/shared/utils/utils.h"

#include <QDebug>
#include <cmath>

TDATracksObject::TDATracksObject(QObject *parent,
                                 TrackBaseRepository *repoTrack,
                                 WeaponAssignService *serviceWA,
                                 WeaponTrackAssignService *serviceWTA,
                                 double scale)
    : TDAZoomableObjectBase{parent},
      arpaRepo(repoTrack),
      waService(serviceWA),
      wtaService(serviceWTA),
      tdaScale(scale),
      currAssignedTrack(0)
{
    parentWidget = dynamic_cast<QWidget *>(parent);
    if (parentWidget == nullptr)
    {
        throw ErrObjectCreation();
    }

    // add this to listener
    TrackRepositoryPublisher *publisher = dynamic_cast<TrackRepositoryPublisher *>(arpaRepo);
    publisher->AddListener(this);

    connect(wtaService, &WeaponTrackAssignService::signal_assignmentResponseData,
            this, &TDATracksObject::OnWeaponAssignment);
    connect(waService, &WeaponAssignService::OnAssignModeChange,
            this, &TDATracksObject::onAssignModeChange);
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
        QPoint cartesian = Utils::polar2Cartesian(
                    tr->getTrackData()->getRange(),
                    tr->getTrackData()->getBearing(),
                    tdaScale,
                    QPoint(parentWidget->width(),parentWidget->height()),
                    QPoint(TRACK_ICON_MARGIN.width(),TRACK_ICON_MARGIN.height())
                    );
        tr->move(cartesian);
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

void TDATracksObject::OnWeaponAssignment(BaseResponse<TrackAssignResponse> resp, bool assign)
{
    if (resp.getHttpCode() == 0)
    {
        TdaTrack *findTrack = trackObjListMap.value(resp.getData().getTrackId(), nullptr);
        if (findTrack)
        {
            if (assign) {
                currAssignedTrack = resp.getData().getTrackId();
            } else {
                currAssignedTrack = 0;
            }

            findTrack->setSelected(assign);
        }
    }
}

void TDATracksObject::onAssignModeChange(const QString &weapon, const WeaponAssign::WeaponAssignMode &mode)
{
    if (mode == WeaponAssign::NONE) {
        TdaTrack *findTrack = trackObjListMap.value(currAssignedTrack, nullptr);
        if (findTrack)
        {
            currAssignedTrack = 0;
            findTrack->setSelected(false);
        }
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
        QPoint cartesian = Utils::polar2Cartesian(
                    findTrack->getTrackData()->getRange(),
                    findTrack->getTrackData()->getBearing(),
                    tdaScale,
                    QPoint(parentWidget->width(),parentWidget->height()),
                    QPoint(TRACK_ICON_MARGIN.width(),TRACK_ICON_MARGIN.height())
                    );
        findTrack->move(cartesian);
    }
}
void TDATracksObject::generateTrackUI(TrackBaseEntity *newTrack)
{
    TdaTrack *tr = new TdaTrack(parentWidget, TRACK_WIDGET_SIZE);
    connect(tr, &TdaTrack::identityChange_Signal, this, &TDATracksObject::OnIdentityChange);

    tr->buildUI(entityToTrackParam(newTrack));
    QPoint cartesian = Utils::polar2Cartesian(
                tr->getTrackData()->getRange(),
                tr->getTrackData()->getBearing(),
                tdaScale,
                QPoint(parentWidget->width(),parentWidget->height()),
                QPoint(TRACK_ICON_MARGIN.width(),TRACK_ICON_MARGIN.height())
                );
    tr->move(cartesian);
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
