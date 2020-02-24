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

    QPen pen;
    pen.setBrush(Qt::NoBrush);
    pen.setStyle(Qt::SolidLine);
    pen.setColor(Qt::yellow);
    painter.setPen(pen);

    int side = qMax(width()/2,height()/2);
    painter.rotate(30);
    painter.drawLine(0,0,0,-side);
    painter.rotate(-30);
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
