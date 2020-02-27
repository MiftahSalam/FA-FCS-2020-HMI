#include "frametda.h"
#include "ui_frametda.h"

#include <QDebug>

FrameTDA::FrameTDA(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::FrameTDA)
{
    ui->setupUi(this);

    timer = new QTimer(this);

    currentHeading = 0.0;

    connect(timer,SIGNAL(timeout()),this,SLOT(update()));
    timer->start(1000);
}

void FrameTDA::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    qDebug()<<Q_FUNC_INFO;

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.drawRect(0,0,width(),height());
    painter.translate(width()/2,height()/2);

    // ==== Compas ==== //
    int range = qMin(width(),height())/4;
    QString text;
    for(int j=0;j<12;j++)
    {
        if(j<9)
            QTextStream(&text)<<(j*30)+90;
        else
            QTextStream(&text)<<(j*30)-270;

        QRect rect((range+20)*((j*30)/180)-15,((range+20)*((j*30)/180)-5),30,15);
        QTextOption opt;
        opt.setAlignment(Qt::AlignHCenter);
        QFont font;

        font.setPixelSize(20);
        painter.setFont(font);
        painter.drawText(rect,text,opt);
        text.clear();
    }

    for(int j=0;j<180;j++)
    {
        if(j%15==0)
            painter.drawLine(0,range-10,0,range+10);
        else
            painter.drawLine(0,range-5,0,range+5);

        painter.rotate(2);
    }

    // ==== Heading Marker ==== //
    int sideMax = qMax(width(),height());
    painter.rotate(currentHeading);
    painter.setPen(QColor(255,255,0,255));
    painter.drawLine(0,0,0,-sideMax);
    painter.rotate(-currentHeading);

    // ==== Gun Coverage ==== //
    const qreal gun_orientation = 180.0;
    const qreal blind_arc = 150.0;
    const qreal gun_fire_range = 7.0; //NM
    int sideMin = qMin(width()/1.8,height()/1.8);
    int span = 360-blind_arc;

    painter.setPen(QColor(255,0,0,255));
    painter.rotate(gun_orientation+currentHeading+qreal((span/2)-90));
    painter.drawPie(-sideMin/2,-sideMin/2,sideMin,sideMin,0*16,span*16);
    painter.rotate(-gun_orientation-currentHeading-qreal((span/2)-90));

    QRect rect(-((sideMin/2)+20)*((currentHeading+100)/180)-15,-((sideMin/2)+20)*((currentHeading+100)/180)-5,35,15);
    QTextOption opt;
    opt.setAlignment(Qt::AlignHCenter);
    QFont font;

    font.setPixelSize(11);
    painter.setFont(font);
    painter.drawText(rect,QString("40mm"),opt);

    /*
    double TTLF_x = query.value(3).toDouble();
    double TTLF_y = query.value(4).toDouble();
    double rng = range2Pixel(query.value(0).toDouble());
    double brn = 90-query.value(1).toDouble();
    int tn = query.value(5).toInt();
    painter.setPen(QColor(0,255,0,255));
    painter.drawLine(0,0,range2Pixel(TTLF_x),-range2Pixel(TTLF_y));
    painter.drawLine(rng*cos(brn*(M_PI/180)),-rng*sin(brn*(M_PI/180)),range2Pixel(TTLF_x),-range2Pixel(TTLF_y));
    painter.drawLine(rng*cos(brn*(M_PI/180)),-rng*sin(brn*(M_PI/180)),0,0);

    statusBarSelectedTrack->showMessage(QString("Tn : %1     "
                                                "Range : %2 NM     "
                                                "Bearing : %3     "
                                                "Speed : %4 kts     "
                                                "Course : %5     "
                                                //"Height : %6 ft     "
                                                )
                                        .arg(tn)
                                        .arg(QString::number(mapTracks->value(tn).trackData.range,'f',2))
                                        .arg(QString::number(mapTracks->value(tn).trackData.bearing,'f',2))
                                        .arg(QString::number(mapTracks->value(tn).trackData.speed,'f',2))
                                        .arg(QString::number(mapTracks->value(tn).trackData.course,'f',2))
                                        );
}
else
    statusBarSelectedTrack->clearMessage();
    */
}

void FrameTDA::setHeading(QString heading)
{
    currentHeading = heading.toFloat();
    qDebug()<<Q_FUNC_INFO<<currentHeading;

}


FrameTDA::~FrameTDA()

{
    delete ui;
}
