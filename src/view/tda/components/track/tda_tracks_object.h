#ifndef TDATRACKSOBJECT_H
#define TDATRACKSOBJECT_H

#include "src/domain/track/repository/track_base_repository.h"
#include "src/usecase/engagement/weapon_track_assign_service.h"
#include "src/usecase/weapon_assign/weapon_assign_service.h"
#include "src/view/tda/components/tda_object_base.h"
#include "src/infra/store/track/track_repository_listener.h"
#include "src/view/tda/components/track/tda_track.h"

class TDATracksObject : public TDAZoomableObjectBase, public TrackRepositoryListener
{
    Q_OBJECT
public:
    explicit TDATracksObject(QObject *parent = nullptr,
                             TrackBaseRepository *repoTrack = nullptr,
                             WeaponAssignService *serviceWA = nullptr,
                             WeaponTrackAssignService *wtaService = nullptr,
                             double scale = 0.);

    ~TDATracksObject();

    // TDAObjectBase interface
    void Draw(QPainter *painter, const int &side, const int &width, const int &height, const QPoint &off_center) override;

    //  TrackRepositoryListener interface
    void OnTracksAdded(std::list<TrackBaseEntity *> tnList) override;
    void OnTracksRemoved(std::list<int> tnIdList) override;
    void OnTrackPropertyChanged(int tn, TrackBaseEntity *track) override;
    void OnTrackSelectedChanged(int tn) override;

public slots:
    // TDAZoomableObjectBase interface
    void OnZoom(float range) override;

    void OnTrackSelectedChange(int tn);
    void OnIdentityChange(int tn, TrackUtils::Identity newIdentity);
    void OnWeaponAssignment(BaseResponse<TrackAssignResponse> resp, bool assign);
    void onAssignModeChange(const QString &weapon, const WeaponAssign::WeaponAssignMode &mode);

private:
    TrackBaseRepository *arpaRepo;
    WeaponAssignService *waService;
    WeaponTrackAssignService *wtaService;

    QMap<int, TdaTrack *> trackObjListMap;

    QWidget *parentWidget;

    double tdaScale;
    int currAssignedTrack;

    void generateTrackUI(TrackBaseEntity *newTrack);
    TrackParam *entityToTrackParam(TrackBaseEntity *track);
};

#endif // TDATRACKSOBJECT_H
