#include "tda_fire_triangle_object.h"

#include <QTextStream>
#include <cmath>
#include <QDebug>

TDAFireTriangleObject::TDAFireTriangleObject(QObject *parent, FireTriangleBaseRepository *repoFireTriangle, TDAConfig *configTDA):
    TDAObjectBase (parent), fireTriangleRepo(repoFireTriangle), tdaConfig(configTDA)
{
    fireTriangleRepo->SetFireTriangle(FireTriangleEntity(60,2000,3000));
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

    int ttlf_x_Pixel = (int)(ttlf_x_NM*(width/2*tdaScale));
    int ttlf_y_Pixel = (int)(ttlf_y_NM*(width/2*tdaScale));

    qDebug()<< "----------------ttlf x"<<ttlf_x_Pixel;
    qDebug()<< "----------------ttlf y"<<ttlf_y_Pixel;

    //posisi track
    float track_x = 50;
    float track_y = 100;



    QPoint points[3] = {
        center_point,
        QPoint(ttlf_x_Pixel,ttlf_y_Pixel), // ttlf
        QPoint(width/2+50, height/2+150) // tracks
    };

    // painter->translate(center_point);
    painter->setPen(QColor(0,0,255,255));
    painter->drawPolygon(points, 3);
    // painter->translate(-center_point);
}

