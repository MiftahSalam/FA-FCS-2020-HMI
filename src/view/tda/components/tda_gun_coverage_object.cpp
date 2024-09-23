#include "tda_gun_coverage_object.h"

#include <QTextStream>
#include <cmath>

TDAGunCoverageObject::TDAGunCoverageObject(QObject *parent, OSDInertiaRepository *repoInertia,
                                           GunCoverageRepository *repoGunCov, TDAConfig *configTDA):
    TDAZoomableObjectBase(parent),
    repoInertia(repoInertia),
    repoGunCov(repoGunCov),
    tdaConfig(configTDA)
{
    repoGunCov->SetGunCoverage(GunCoverageEntity(10000,120,0));
}

void TDAGunCoverageObject::Draw(QPainter *painter, const int &side, const int &width, const int &height, const QPoint &off_center)
{
    QPoint center_point = QPoint(width/2,height/2);

    if(tdaConfig->getGunCoverageStatus() == true)
    {
        tdaScale = tdaConfig->getZoomScale();
        float gun_orientation = repoGunCov->GetGunCoverage()->getGunOrientation();
        float blind_arc = repoGunCov->GetGunCoverage()->getBlindArc();
        float max_range = repoGunCov->GetGunCoverage()->getMax_range();
        // konversi ke NM
        float max_range_NM = max_range/1852;
        float currHeading = repoInertia->GetInertia()->heading();

        int span = 360-blind_arc;
        int range2pixel = (int)(max_range_NM*(side/(tdaScale)));
        int gun_coveragePixel = 2*range2pixel;

        painter->translate(center_point);
        painter->setPen(QColor(255,0,0,255));
        painter->rotate(gun_orientation+currHeading+qreal((span/2)-90));
        painter->drawPie(-gun_coveragePixel/2,-gun_coveragePixel/2,gun_coveragePixel,
                         gun_coveragePixel,0*16,span*16);
        painter->rotate(-gun_orientation-currHeading-qreal((span/2)-90));

        QRect rect(-((gun_coveragePixel/2)+20)*cos((currHeading+100)*M_PI/180)-15,-((gun_coveragePixel/2)+20)*sin((currHeading+100)*M_PI/180)-5,30,15);
        QTextOption opt;
        opt.setAlignment(Qt::AlignHCenter);
        QFont font;

        font.setPixelSize(11);
        painter->setFont(font);
        painter->drawText(rect,QString("40mm"),opt);

        painter->translate(-center_point);
    }
}



void TDAGunCoverageObject::OnZoom(float range)
{
}
