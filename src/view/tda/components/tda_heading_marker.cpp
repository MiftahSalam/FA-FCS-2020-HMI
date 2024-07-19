#include "tda_heading_marker.h"

#include "qmath.h"
#include <QTextStream>
#include <cmath>

TDAHeadingMarker::TDAHeadingMarker(QObject *parent): TDAObjectBase(parent)
{

}

void TDAHeadingMarker::Draw(QPainter *painter, const int &side, const int &width, const int &height, const QPoint &off_center)
{
    QPoint center_point = QPoint(width/2,height/2);

    painter->drawLine(10,10,100,100);

}

