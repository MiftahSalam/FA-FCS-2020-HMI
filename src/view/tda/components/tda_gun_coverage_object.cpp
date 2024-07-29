#include "tda_gun_coverage_object.h"

#include "qmath.h"
#include <QTextStream>
#include <cmath>

TDAGunCoverageObject::TDAGunCoverageObject(QObject *parent): TDAObjectBase(parent)
{

}

void TDAGunCoverageObject::Draw(QPainter *painter, const int &side, const int &width, const int &height, const QPoint &off_center)
{
    QPoint center_point = QPoint(width/2,height/2);
    const bool show_gunCoverage = true;

    if(show_gunCoverage)
    {
        // const qreal gun_orientation = QString(orientation).toDouble();
        // const qreal blind_arc = QString(blind_arc1).toDouble();
        // const qreal max_range = QString(max_range1).toDouble(); //NM

        double gun_orientation = 0; //posisi gun haluan
        double blind_arc = 90;
        // double max_range = 2;
        double currHeading = 45;

        int span = 360-blind_arc;
        int gun_coveragePixel = 600;


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
