#ifndef TDA_FIRE_TRIANGLE_OBJECT_H
#define TDA_FIRE_TRIANGLE_OBJECT_H

#include <QObject>

#include "src/domain/track/repository/track_base_repository.h"
#include "src/usecase/engagement/weapon_track_assign_service.h"
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
                          WeaponTrackAssignService *serviceWTA = nullptr,
                          TDAConfig *configTDA = nullptr
            );

    void Draw(QPainter *painter, const int &side, const int &width, const int &height, const QPoint &off_center) override;

private:
    FireTriangleBaseRepository *fireTriangleRepo;
    TrackBaseRepository *arpaRepo;
    WeaponTrackAssignService *wtaSercie;
    TDAConfig *tdaConfig;

    double tdaScale;

};

#endif // TDA_FIRE_TRIANGLE_OBJECT_H
