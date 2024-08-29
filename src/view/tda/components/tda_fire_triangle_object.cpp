#include "tda_fire_triangle_object.h"

#include <QTextStream>
#include <cmath>

TDAFireTriangleObject::TDAFireTriangleObject(QObject *parent, FireTriangleBaseRepository *repoFireTriangle):
    TDAObjectBase (parent), fireTriangleRepo(repoFireTriangle)
{
    fireTriangleRepo->SetFireTriangle(FireTriangleEntity(60,100,200));
}

void TDAFireTriangleObject::Draw(QPainter *painter, const int &side, const int &width, const int &height, const QPoint &off_center)
{
    QPoint center_point = QPoint(width/2,height/2);
    float ttlf = fireTriangleRepo->GetFireTriangle()->getTTLF();
    float ttlf_x = fireTriangleRepo->GetFireTriangle()->getTTLFX();
    float ttlf_y = fireTriangleRepo->GetFireTriangle()->getTTLFY();

    //posisi track
    float track_x = 50;
    float track_y = 70;

    painter->translate(center_point);
    painter->setPen(QColor(0,0,255,255));

}
