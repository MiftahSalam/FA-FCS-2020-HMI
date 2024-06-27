#include "tda_compass_object.h"

#include "qmath.h"
#include <QTextStream>
#include <cmath>

TdaCompassObject::TdaCompassObject(QObject *parent): TDAObjectBase(parent)
{

}


void TdaCompassObject::Draw(QPainter *painter, const int &side, const int &width, const int &height, const QPoint &off_center)
{
    QPoint center_point = QPoint(width/2,height/2);

    //TODO: get from config
    const bool show_compass = true;
    if(show_compass)
    {
        painter->translate(center_point);

        //TODO: get from config
        const bool heading_up = true;

        //TODO: get from Inertia Domain
        const double bearing = 0.;

        QPen curPen = painter->pen();

        /*
         * compass ring text
        */
        int brnCor = 0;
        int brn;
        double buf_j;
        int margin_a = 5;
        int margin_b = 15;

        if(heading_up)
            brnCor =  -static_cast<int>(bearing);

        QString text;
        for(int j=0;j<12;j++)
        {
            if(j<9)
                brn = (j*30)+90;
            else
                brn = (j*30)-270;

            QTextStream(&text)<<brn;

            buf_j = (j*30)+brnCor;

            /*
            */
            while(buf_j>360 || buf_j<0)
            {
                if(buf_j>360)
                    buf_j -= 360;
                if(buf_j<0)
                    buf_j += 360;
            }

            double x = static_cast<double>(side-30)*cos((buf_j)*M_PI/180);
            double y = static_cast<double>(side-30)*sin((buf_j)*M_PI/180);
            QRect rect(static_cast<int>(x)-margin_b,static_cast<int>(y)-margin_a,30,15);
            QTextOption opt;
            opt.setAlignment(Qt::AlignHCenter);
            QFont font;

            font.setPixelSize(12);
            painter->setFont(font);
            painter->drawText(rect,text,opt);
            text.clear();
        }

        /*
         * compass ring
        */
        if(heading_up)
            painter->rotate(-bearing);

        for(int j=0;j<180;j++)
        {
            margin_a = 10;
            margin_b = 5;


            QPoint p1(-center_point+off_center);
            //                QPoint p1(-off_center.x(),-off_center.y());
            //            QPoint p1(-height/4,-height/4);
            //            QPoint p1(0,-height/4);
            QPoint p2((side-10)*qCos(j*2*M_PI/180.),(side-10)*qSin(j*2*M_PI/180.));
            QLineF line1(p1,p2);
            QLineF line2;
            //            QPoint p3 = line1.pointAt(0.95).toPoint();

            line2.setP1(p2);
            line2.setAngle(line1.angle());
            //            line2.setLength(-10);
            //            qDebug()<<Q_FUNC_INFO<<"j"<<j<<"p2"<<p2;
            //            qDebug()<<Q_FUNC_INFO<<"line2"<<line2;

            if(j%15==0)
            {
                line2.setLength(-margin_a);
                painter->drawLine(line2);
            }
            else
            {
                line2.setLength(-margin_b);
                painter->drawLine(line2);
            }
        }

        if(heading_up)
            painter->rotate(bearing);

        painter->setPen(curPen);

        painter->translate(-center_point);
    }
}
