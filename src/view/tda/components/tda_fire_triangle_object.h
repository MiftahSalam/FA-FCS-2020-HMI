#ifndef TDA_FIRE_TRIANGLE_OBJECT_H
#define TDA_FIRE_TRIANGLE_OBJECT_H

#include <QObject>

#include "src/domain/track/repository/track_base_repository.h"
#include "src/usecase/engagement/weapon_track_assign_service.h"
#include "src/usecase/weapon_assign/weapon_assign_service.h"
#include "tda_object_base.h"
#include "src/domain/fire_triangle/repository/fire_triangle_repository.h"
#include "src/shared/config/tda_config.h"

class TDAFireTriangleObject :public TDAObjectBase
{
    Q_OBJECT
public:
    TDAFireTriangleObject(QObject *parent = nullptr,
                          FireTriangleBaseRepository *repoFireTriangle = nullptr,
                          TrackBaseRepository *repoArpa = nullptr,
                          WeaponAssignService *serviceWA = nullptr,
                          WeaponTrackAssignService *serviceWTA = nullptr,
                          TDAConfig *configTDA = nullptr
            );

    void Draw(QPainter *painter, const int &side, const int &width, const int &height, const QPoint &off_center) override;

private slots:
    void OnWeaponAssignment(BaseResponse<TrackAssignResponse> resp, bool assign);
    void onAssignModeChange(const QString &weapon, const WeaponAssign::WeaponAssignMode &mode);

private:
    FireTriangleBaseRepository *fireTriangleRepo;
    TrackBaseRepository *arpaRepo;
    WeaponAssignService *waService;
    WeaponTrackAssignService *wtaSercie;
    TDAConfig *tdaConfig;

    double tdaScale;
    int currAssignedTrack;

};

#endif // TDA_FIRE_TRIANGLE_OBJECT_H
