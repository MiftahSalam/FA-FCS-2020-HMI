#ifndef TDA_GUN_COVERAGE_OBJECT_H
#define TDA_GUN_COVERAGE_OBJECT_H

#include <QObject>

#include "tda_object_base.h"
#include "src/domain/osd/repository/osd_inertia_repository.h"
#include "src/domain/gun/repository/gun_coverage_repository.h"
#include "src/shared/config/tda_config.h"

class TDAGunCoverageObject: public TDAZoomableObjectBase
{
    Q_OBJECT
public:
    TDAGunCoverageObject(QObject* parent = nullptr, OSDInertiaRepository* repoInertia = nullptr,
                         GunCoverageRepository *repoGunCov = nullptr, TDAConfig *configTDA = nullptr);

public:
    void Draw(QPainter *painter, const int &side, const int &width, const int &height, const QPoint &off_center) override;

private:
    OSDInertiaRepository *repoInertia;
    GunCoverageRepository *repoGunCov;
    TDAConfig *tdaConfig;


    // TDAZoomableObjectBase interface
public slots:
    void OnZoom(float range) override;
};

#endif // TDA_GUN_COVERAGE_OBJECT_H
