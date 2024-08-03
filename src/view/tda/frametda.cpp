#include "frametda.h"
#include "src/di/di.h"
#include "src/view/tda/components/tda_compass_object.h"
#include "src/view/tda/components/tda_gun_coverage_object.h"
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

    TdaCompassObject *compass = new TdaCompassObject(this, config->getTDAConfig());
    TDAGunCoverageObject *gunCoverage = new TDAGunCoverageObject(this, osdRepo->getRepoOSDInertia(), gunRepo->getRepoGunCoverage(), config->getTDAConfig());
    TDAHeadingMarkerObject *headingMarker = new TDAHeadingMarkerObject (this, osdRepo->getRepoOSDInertia());
    TDAGunBarrelObject *gunBarrel = new TDAGunBarrelObject (this, osdRepo->getRepoOSDInertia(), gunRepo->getRepoGunFeedback());
    TDATracksObject *tracksObject = new TDATracksObject(this);

    objectItems << compass << tracksObject << headingMarker << gunBarrel << gunCoverage;

    timer->start(1000);

    ZoomSubMenu = new QMenu("Zoom",this);
    cur_checked_zoom_scale = zoomScale2Int(Z_080);

    for (int i=0;i<Z_TOTAL;i++)
    {
        ZoomAction[i] = new QAction(zoomScale2String(zoomInt2Scale(i)),this);
        ZoomSubMenu->addAction(ZoomAction[i]);
        ZoomAction[i]->setCheckable(true);
        connect(ZoomAction[i], &QAction::triggered, this, &FrameTDA::onZoomChange);
    }
    ZoomAction[cur_checked_zoom_scale]->setChecked(true);

    CompassAction = new QAction("Show Compass", this);
    HeadingMarkerAction = new QAction("Show Heading Marker", this);
    GunCovAction = new QAction("Show Gun Coverage", this);
    GunBarrelAction = new QAction("Show Gun Barrel",this);

    CompassAction->setCheckable(true);
    HeadingMarkerAction->setCheckable(true);
    GunCovAction->setCheckable(true);
    GunBarrelAction->setCheckable(true);

    if (config->getTDAConfig()->getInstance("")->getCompassStatus() == "true")
        CompassAction->setChecked(true);

    if (config->getTDAConfig()->getInstance("")->getGunCoverageStatus() == "true")
        GunCovAction->setChecked(true);

    if (config->getTDAConfig()->getInstance("")->getHeadingMarkerStatus() == "true")
        HeadingMarkerAction->setChecked(true);

    if (config->getTDAConfig()->getInstance("")->getGunBarrelStatus() == "true")
        GunBarrelAction->setChecked(true);

    connect(this, SIGNAL(signalOnCostumContextMenuRequest(QPoint&pos)), this, SLOT(on_FrameTDA_customContextMenuRequested(QPoint&pos)));
    connect(CompassAction, &QAction::triggered, this, &FrameTDA::onCompassActionTriggered);
    connect(HeadingMarkerAction, &QAction::triggered, this, &FrameTDA::onHeadingMarkerActionTriggrered);
    connect(GunCovAction, &QAction::triggered, this, &FrameTDA::onGunCovActionTriggered);
    connect(GunBarrelAction, &QAction::triggered, this, &FrameTDA::onGunBarrelActionTriggered);

    connect(this, SIGNAL(signalOnShowCompassObject(bool)), compass, SLOT(OnShowCompass(bool)));
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

void FrameTDA::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
    {
        this->setContextMenuPolicy(Qt::CustomContextMenu);
        emit signalOnCostumContextMenuRequest(event->pos());
    }
}

void FrameTDA::timeOut()
{
    update();

    auto pos = osdRepo->getRepoOSDPosition()->GetPosition(); //temp test
    auto inertia = osdRepo->getRepoOSDInertia()->GetInertia(); //temp test
    qDebug()<<Q_FUNC_INFO<<pos->latitude()<<inertia->heading();
}

void FrameTDA::on_FrameTDA_customContextMenuRequested(const QPoint &pos)
{
    QMenu *menu = new QMenu(this);

    menu->addMenu(ZoomSubMenu);
    menu->addAction(CompassAction);
    menu->addAction(HeadingMarkerAction);
    menu->addAction(GunCovAction);
    menu->addAction(GunBarrelAction);
    menu->exec(pos);
}

void FrameTDA::onCompassActionTriggered()
{
    if (CompassAction->isChecked() == true)
    {
        config->getTDAConfig()->getInstance("")->setCompassStatus("true");
    }else{
        config->getTDAConfig()->getInstance("")->setCompassStatus("false");
    }
}

void FrameTDA::onHeadingMarkerActionTriggrered()
{
    if (HeadingMarkerAction->isChecked() == true)
    {
        QMessageBox::information(this, "HM Action", "HM triggered"); //temp
    }
}

void FrameTDA::onGunCovActionTriggered()
{
    if (GunCovAction->isChecked() == true)
    {
        config->getTDAConfig()->getInstance("")->setGunCoverageStatus("true");
    }else{
        config->getTDAConfig()->getInstance("")->setGunCoverageStatus("false");
    }
}

void FrameTDA::onGunBarrelActionTriggered()
{
    if (GunBarrelAction->isChecked() == true)
    {
        QMessageBox::information(this, "Gun Barrel Action", "Gun Barrel triggered"); //temp
    }
}

void FrameTDA::onZoomChange()
{
    for(int i=0;i<Z_TOTAL;i++)
    {
        if(ZoomAction[i]->isChecked() && i != cur_checked_zoom_scale)
        {
            ZoomAction[cur_checked_zoom_scale]->setChecked(false);
            cur_checked_zoom_scale = i;
        }
    }
    QMessageBox::information(this, "Zoom", QString::number(cur_checked_zoom_scale)); // temp
    ZoomAction[cur_checked_zoom_scale]->setChecked(true);
    tdaScale = ZoomAction[cur_checked_zoom_scale]->text().remove(" NM").toDouble();
}

QString FrameTDA::zoomScale2String(zoomScale scale)
{
    if(scale==Z_025)
        return "0.25 NM";
    else if(scale==Z_050)
        return "0.5 NM";
    else if(scale==Z_010)
        return "1 NM";
    else if(scale==Z_020)
        return "2 NM";
    else if(scale==Z_040)
        return "4 NM";
    else if(scale==Z_080)
        return "8 NM";
    else if(scale==Z_160)
        return "16 NM";
    else if(scale==Z_320)
        return "32 NM";
    else if(scale==Z_640)
        return "64 NM";
    else
        return "0";
}

FrameTDA::zoomScale FrameTDA::zoomStrig2Scale(QString scale)
{
    if(scale=="0.25 NM")
        return Z_025;
    else if(scale=="0.5 NM")
        return Z_050;
    else if(scale=="1 NM")
        return Z_010;
    else if(scale=="2 NM")
        return Z_020;
    else if(scale=="4 NM")
        return Z_040;
    else if(scale=="8 NM")
        return Z_080;
    else if(scale=="16 NM")
        return Z_160;
    else if(scale=="32 NM")
        return Z_320;
    else if(scale=="64 NM")
        return Z_640;
    else
        return Z_TOTAL;
}

int FrameTDA::zoomScale2Int(zoomScale scale)
{
    if(scale==Z_025)
        return 0;
    else if(scale==Z_050)
        return 1;
    else if(scale==Z_010)
        return 2;
    else if(scale==Z_020)
        return 3;
    else if(scale==Z_040)
        return 4;
    else if(scale==Z_080)
        return 5;
    else if(scale==Z_160)
        return 6;
    else if(scale==Z_320)
        return 7;
    else if(scale==Z_640)
        return 8;
}

FrameTDA::zoomScale FrameTDA::zoomInt2Scale(int scale)
{
    if(scale==0)
        return Z_025;
    else if(scale==1)
        return Z_050;
    else if(scale==2)
        return Z_010;
    else if(scale==3)
        return Z_020;
    else if(scale==4)
        return Z_040;
    else if(scale==5)
        return Z_080;
    else if(scale==6)
        return Z_160;
    else if(scale==7)
        return Z_320;
    else if(scale==8)
        return Z_640;
    else
        return Z_TOTAL;
}

