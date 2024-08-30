#include "tda_fire_triangle_object.h"

#include <QTextStream>
#include <cmath>
#include <QDebug>

TDAFireTriangleObject::TDAFireTriangleObject(QObject *parent, FireTriangleBaseRepository *repoFireTriangle, TDAConfig *configTDA):
    TDAObjectBase (parent), fireTriangleRepo(repoFireTriangle), tdaConfig(configTDA)
{
    fireTriangleRepo->SetFireTriangle(FireTriangleEntity(60,2000,2000));
}

void TDAFireTriangleObject::Draw(QPainter *painter, const int &side, const int &width, const int &height, const QPoint &off_center)
{
    tdaScale = tdaConfig->getZoomScale();
    QPoint center_point = QPoint(width/2,height/2);
    float ttlf = fireTriangleRepo->GetFireTriangle()->getTTLF();
    float ttlf_x = fireTriangleRepo->GetFireTriangle()->getTTLFX();
    float ttlf_y = fireTriangleRepo->GetFireTriangle()->getTTLFY();
    //konversi ke NM
    float ttlf_x_NM = ttlf_x/1852;
    float ttlf_y_NM = ttlf_y/1852;

    int ttlf_x_Pixel = (int)(ttlf_x_NM*(side/tdaScale));
    int ttlf_y_Pixel = (int)(ttlf_y_NM*(side/tdaScale));

    //posisi track
    float track_x = 5000;
    float track_y = 2000;

    //konversi ke NM
    float track_x_NM = track_x/1852;
    float track_y_NM = track_y/1852;

    int track_x_Pixel = (int)(track_x_NM*(side/tdaScale));
    int track_y_Pixel = (int)(track_y_NM*(side/tdaScale));

    painter->translate(center_point);
    painter->setPen(QColor(30,144,255,255));
    painter->drawLine(0,0, ttlf_x_Pixel, -ttlf_y_Pixel);
    painter->drawLine(ttlf_x_Pixel, -ttlf_y_Pixel, track_x_Pixel, -track_y_Pixel);
    painter->drawLine(track_x_Pixel, -track_y_Pixel, 0, 0);
    painter->translate(-center_point);
}

