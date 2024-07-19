#include "tda_heading_marker_object.h"

TDAHeadingMarkerObject::TDAHeadingMarkerObject(QObject *parent): TDAObjectBase (parent){}

void TDAHeadingMarkerObject::Draw(QPainter *painter, const int &side, const int &width, const int &height, const QPoint &off_center)
{
    QPoint center_point = QPoint(width/2,height/2);

    const bool show_headingMarker = true;
    if (show_headingMarker)
    {
        painter->translate(center_point);

        const double heading = 0;

        // int sideMax = qMax (width(),height());
        painter->setPen(QColor(255,255,0,255));
        painter->drawLine(0,0,0, 250);

    }
}
