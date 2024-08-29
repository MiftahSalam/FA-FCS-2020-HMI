#ifndef TDA_FIRE_TRIANGLE_OBJECT_H
#define TDA_FIRE_TRIANGLE_OBJECT_H

#include <QObject>

#include "tda_object_base.h"
#include "src/domain/fire_triangle/repository/fire_triangle_repository.h"

class TDAFireTriangleObject :public TDAObjectBase
{
    Q_OBJECT
public:
    TDAFireTriangleObject(QObject *parent = nullptr,
                          FireTriangleBaseRepository *repoFireTriangle = nullptr);

    void Draw(QPainter *painter, const int &side, const int &width, const int &height, const QPoint &off_center) override;

private:
    FireTriangleBaseRepository *fireTriangleRepo;
};

#endif // TDA_FIRE_TRIANGLE_OBJECT_H
