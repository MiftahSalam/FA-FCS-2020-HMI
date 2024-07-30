#include "frametda.h"
#include "src/di/di.h"
#include "src/view/tda/components/tda_compass_object.h"
#include "src/view/tda/components/tda_heading_marker_object.h"
#include "src/view/tda/components/tda_gun_barrel_object.h"
#include "src/view/tda/components/track/tda_tracks_object.h"
#include "ui_frametda.h"

#include <math.h>
#include <QContextMenuEvent>
#include <QMouseEvent>
#include <QDebug>
#include <QMessageBox>
#include <QDateTime>

FrameTDA::FrameTDA(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::FrameTDA)
{
    ui->setupUi(this);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &FrameTDA::timeOut);

    //TODO: setup status bar (mouse & track selected)

    //TODO: setup event filter

    //TODO: setup popup menu

    osdRepo = DI::getInstance()->getRepoOSD(); //temp
    gunRepo = DI::getInstance()->getRepoGun();

    TdaCompassObject *compass = new TdaCompassObject(this);
    TDAHeadingMarkerObject *headingMarker = new TDAHeadingMarkerObject (this, osdRepo->getRepoOSDInertia());
    TDAGunBarrelObject *gunBarrel = new TDAGunBarrelObject (this, osdRepo->getRepoOSDInertia(), gunRepo->getRepoGunFeedback());
    TDATracksObject *tracksObject = new TDATracksObject(this);
    objectItems << compass << tracksObject << headingMarker << gunBarrel;


    timer->start(1000);
}

FrameTDA::~FrameTDA()
{
    delete ui;
}

void FrameTDA::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    QPoint center_point = QPoint(this->width() / 2, this->height() / 2);
    int side = qMin(this->width(), this->height()) / 2;

    painter.setRenderHint(QPainter::Antialiasing);
    painter.drawRect(0,0, width(), height());

    foreach (TDAObjectBase *obj, objectItems)
    {
        obj->Draw(&painter, side, this->width(), this->height(), center_point);
    }
}

void FrameTDA::timeOut()
{
    update();

    auto pos = osdRepo->getRepoOSDPosition()->GetPosition(); //temp test
    auto inertia = osdRepo->getRepoOSDInertia()->GetInertia(); //temp test
    qDebug()<<Q_FUNC_INFO<<pos->latitude()<<inertia->heading();
}
