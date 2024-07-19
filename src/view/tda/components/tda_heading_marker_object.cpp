#include "tda_heading_marker_object.h"

TDAHeadingMarkerObject::TDAHeadingMarkerObject(QObject *parent): TDAObjectBase (parent){}

void TDAHeadingMarkerObject::Draw(QPainter *painter, const int &side, const int &width, const int &height, const QPoint &off_center)
{
    QPoint center_point = QPoint(width/2,height/2);

    const bool show_headingMarker = true;
    // const double heading = 90;

    if (show_headingMarker)
    {
        painter->translate(center_point);

        const double heading = 300;

        double drawHeading = heading + 180;
        if (drawHeading > 180)
            drawHeading = drawHeading - 360;
        // int sideMax = qMax (width(),height());
        // painter->rotate(-static_cast<qreal>(heading));
        // painter->translate(center_point);
        painter->rotate(drawHeading);
        painter->setPen(QColor(255,255,0,255));
        painter->drawLine(0,0,0, 1000);

    }
}
