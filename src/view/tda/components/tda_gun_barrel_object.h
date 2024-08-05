#ifndef TDA_GUN_BARREL_OBJECT_H
#define TDA_GUN_BARREL_OBJECT_H

#include <QObject>

#include "tda_object_base.h"
#include "src/domain/osd/repository/osd_inertia_repository.h"
#include "src/domain/gun/repository/gun_feedback_repository.h"
#include "src/shared/config/tda_config.h"

class TDAGunBarrelObject: public TDAObjectBase
{
    Q_OBJECT
public:
    TDAGunBarrelObject(QObject* parent = nullptr, OSDInertiaRepository *repoInertia = nullptr, GunFeedbackRepository *repoGunFeedback = nullptr,
TDAConfig *configTDA = nullptr);

    void Draw(QPainter *painter, const int &side, const int &width, const int &height, const QPoint &off_center) override;

private:
    OSDInertiaRepository *inertiaRepo;
    GunFeedbackRepository *gunFeedbackRepo;
    TDAConfig *tdaConfig;
};

#endif // TDA_GUN_BARREL_OBJECT_H
