#include "tda_heading_marker_object.h"

#include <QTextStream>
#include <cmath>

TDAHeadingMarkerObject::TDAHeadingMarkerObject(QObject *parent, OSDInertiaRepository *repoInertia, TDAConfig *configTDA):
    TDAObjectBase (parent), inertiaRepo(repoInertia), tdaConfig(configTDA)
{
}

void TDAHeadingMarkerObject::Draw(QPainter *painter, const int &side, const int &width, const int &height, const QPoint &off_center)
{
    Q_UNUSED(side);
    Q_UNUSED(off_center);

    QPoint center_point = QPoint(width/2,height/2);
    int sideMax = qMax(width,height);

    if (tdaConfig->getHeadingMarkerStatus() == true)
    {
        painter->translate(center_point);

        const float heading = inertiaRepo->GetInertia()->heading();
        double drawHeading = heading + 180;

        if (drawHeading > 180)
            drawHeading = drawHeading - 360;
        painter->rotate(drawHeading);

        QPen linePen(QColor(255,255,0,255));
        linePen.setWidth(0);
        painter->setPen(linePen);
        painter->drawLine(0,0,0, sideMax);
        painter->rotate(-drawHeading);

        painter->translate(-center_point);
    }
}


