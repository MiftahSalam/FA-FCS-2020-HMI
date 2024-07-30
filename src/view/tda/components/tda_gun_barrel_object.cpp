#include "tda_gun_barrel_object.h"

#include <QTextStream>
#include <cmath>

TDAGunBarrelObject::TDAGunBarrelObject(QObject *parent, OSDInertiaRepository *repoInertia, GunFeedbackRepository *repoGunFeedback): TDAObjectBase (parent), inertiaRepo(repoInertia),
    gunFeedbackRepo(repoGunFeedback)
{
    // inertiaRepo->SetInertia(OSDInertiaEntity(30,0,0,"","",OSD_MODE::AUTO));
    gunFeedbackRepo->SetBarrel(15,0);
}

void TDAGunBarrelObject::Draw(QPainter *painter, const int &side, const int &width, const int &height, const QPoint &off_center)
{
    QPoint center_point = QPoint(width/2,height/2);
    painter->translate(center_point);

    int sideMax = qMax(width,height);
    // const double bearing = 15;
    const float bearing = gunFeedbackRepo->GetBarrel()->azimuth();

    const float heading = inertiaRepo->GetInertia()->heading();
    // double heading = 0;
    double drawHeading = heading + 180;

    if (drawHeading > 180)
        drawHeading = drawHeading - 360;

    painter->rotate(drawHeading+bearing);
    painter->setPen(QPen(Qt::green,5));
    painter->drawLine(0,0,0,(sideMax/20));
    painter->rotate(-drawHeading-bearing);
    painter->translate(-center_point);

}
