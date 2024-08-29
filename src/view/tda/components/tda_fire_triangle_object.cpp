#include "tda_fire_triangle_object.h"

#include <QTextStream>
#include <cmath>

TDAFireTriangleObject::TDAFireTriangleObject(QObject *parent, FireTriangleBaseRepository *repoFireTriangle):
    TDAObjectBase (parent), fireTriangleRepo(repoFireTriangle)
{
    fireTriangleRepo->SetFireTriangle(FireTriangleEntity(60,10,20));
}

void TDAFireTriangleObject::Draw(QPainter *painter, const int &side, const int &width, const int &height, const QPoint &off_center)
{
    QPoint center_point = QPoint(width/2,height/2);
    float ttlf = fireTriangleRepo->GetFireTriangle()->getTTLF();
    float ttlf_x = fireTriangleRepo->GetFireTriangle()->getTTLFX();
    float ttlf_y = fireTriangleRepo->GetFireTriangle()->getTTLFY();

    //posisi track
    float track_x = 50;
    float track_y = 100;

    QPoint points[3] = {
        center_point,
        QPoint(ttlf_x,ttlf_y),
        QPoint(track_x, track_y)
    };

    painter->translate(center_point);
    painter->setPen(QColor(0,0,255,255));
    painter->drawPolygon(points, 3);
    painter->translate(-center_point);
}
