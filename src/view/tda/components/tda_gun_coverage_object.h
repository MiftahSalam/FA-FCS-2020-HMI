#ifndef TDA_GUN_COVERAGE_OBJECT_H
#define TDA_GUN_COVERAGE_OBJECT_H

#include <QObject>

#include "tda_object_base.h"

class TDAGunCoverageObject: public TDAObjectBase
{
    Q_OBJECT
public:
    TDAGunCoverageObject(QObject* parent = nullptr);

public:
    void Draw(QPainter *painter, const int &side, const int &width, const int &height, const QPoint &off_center) override;
};

#endif // TDA_GUN_COVERAGE_OBJECT_H
