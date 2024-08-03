#include "tda_compass_object.h"

#include "qmath.h"
#include <QTextStream>
#include <cmath>
#include <QMessageBox>

TdaCompassObject::TdaCompassObject(QObject *parent, TDAConfig *config): TDAObjectBase(parent), tdaConfig(config)
{

}

void TdaCompassObject::OnShowCompass(bool status)
{
    // show_compass = status;
}

void TdaCompassObject::Draw(QPainter *painter, const int &side, const int &width, const int &height, const QPoint &off_center)
{
    QPoint center_point = QPoint(width/2,height/2);

    //TODO: get from config
    // const bool show_compass = true;
    bool show_compass;
    if (tdaConfig->getInstance("")->getCompassStatus() == "true")
    {
        show_compass = true;
    }else{
        return;
    }

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
        int headingCorrection = 0;
        int brn;
        double buf_j;
        int margin_x = 15;
        int margin_y = 5;
        int textWidth = 30;
        int textheight = 15;
        int textInterval = 12;
        int textIntervalDegree = 360/textInterval;
        int degreeAccumulator = 0;

        if(heading_up)
            headingCorrection =  -static_cast<int>(bearing);

        QString text;
        for(int j=0;j<textInterval;j++)
        {
            if(degreeAccumulator < 270)
                brn = (j*textIntervalDegree)+90;
            else
                brn = (j*textIntervalDegree)-270;

            QTextStream(&text)<<brn;

            buf_j = (j*textIntervalDegree)+headingCorrection;

            /*
            */
            while(buf_j>360 || buf_j<0)
            {
                if(buf_j>360)
                    buf_j -= 360;
                if(buf_j<0)
                    buf_j += 360;
            }

            double x = static_cast<double>(side-textWidth)*cos((buf_j)*M_PI/180);
            double y = static_cast<double>(side-textWidth)*sin((buf_j)*M_PI/180);
            QRect rect(static_cast<int>(x)-margin_x,static_cast<int>(y)-margin_y,textWidth,textheight);
            QTextOption opt;
            opt.setAlignment(Qt::AlignHCenter);
            QFont font;

            font.setPixelSize(12);
            painter->setFont(font);
            painter->drawText(rect,text,opt);
            text.clear();

            degreeAccumulator += textIntervalDegree;
        }

        /*
         * compass ring
        */
        if(heading_up)
            painter->rotate(-bearing);

        int tickStep = 2;
        int tickStepLong = textIntervalDegree;
        int tickStepIteration = 360/tickStep;
        for(int j=0;j<tickStepIteration;j++)
        {
            margin_y = 10;
            margin_x = 5;

            QPoint p1(-center_point+off_center);
            QPoint p2((side-10)*qCos(j*tickStep*M_PI/180.),(side-10)*qSin(j*tickStep*M_PI/180.));
            QLineF line1(p1,p2);
            QLineF line2;

            line2.setP1(p2);
            line2.setAngle(line1.angle());

            if(j%tickStepLong==0)
            {
                line2.setLength(-margin_y);
                painter->drawLine(line2);
            }
            else
            {
                line2.setLength(-margin_x);
                painter->drawLine(line2);
            }
        }

        if(heading_up)
            painter->rotate(bearing);

        painter->setPen(curPen);

        painter->translate(-center_point);
    }
}
