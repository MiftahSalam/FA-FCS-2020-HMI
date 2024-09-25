#include "frametda.h"
#include "src/di/di.h"
#include "src/shared/utils/utils.h"
#include "src/view/tda/components/tda_compass_object.h"
#include "src/view/tda/components/tda_gun_coverage_object.h"
#include "src/view/tda/components/tda_heading_marker_object.h"
#include "src/view/tda/components/tda_gun_barrel_object.h"
#include "src/view/tda/components/track/tda_tracks_object.h"
#include "src/view/tda/components/tda_fire_triangle_object.h"

#include "ui_frametda.h"

#include <math.h>
#include <QContextMenuEvent>
#include <QMouseEvent>
#include <QDebug>
#include <QMessageBox>
#include <QDateTime>

FrameTDA::FrameTDA(QWidget *parent) : QFrame(parent),
                                      ui(new Ui::FrameTDA)
{
    ui->setupUi(this);

    // setup DI
    setupDI();

    // setup TDA Objects
    setupTdaObjects();

    // setup status bar (mouse & track selected)
    setupStatusBar();

    // setup popup menu
    setupContextMenu();

    setMouseTracking(true);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &FrameTDA::timeOut);
    timer->start(1000);
}

FrameTDA::~FrameTDA()
{
    tdaConfig->saveTDAConfig();
    qDebug() << Q_FUNC_INFO << "Save TDA Config";
    delete ui;
}

void FrameTDA::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    QPoint center_point = QPoint(this->width() / 2, this->height() / 2);
    int side = qMin(this->width(), this->height()) / 2;

    painter.setRenderHint(QPainter::Antialiasing);
    painter.drawRect(0, 0, width(), height());

    foreach (TDAObjectBase *obj, objectItems)
    {
        obj->Draw(&painter, side, this->width(), this->height(), center_point);
    }
}

void FrameTDA::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
    {
        emit signalOnCostumContextMenuRequest(event->pos());
    }
}

void FrameTDA::timeOut()
{
    update();

    setupDateTime();
}

void FrameTDA::on_FrameTDA_customContextMenuRequested(const QPoint &pos)
{
    QMenu *menu = new QMenu(this);
    menu->setStyleSheet("background-color: black;");

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
        tdaConfig->setCompassStatus(true);
    }
    else
    {
        tdaConfig->setCompassStatus(false);
    }
    update();
}

void FrameTDA::onHeadingMarkerActionTriggrered()
{
    if (HeadingMarkerAction->isChecked() == true)
    {
        tdaConfig->setHeadingMarkerStatus(true);
    }
    else
    {
        tdaConfig->setHeadingMarkerStatus(false);
    }
    update();
}

void FrameTDA::onGunCovActionTriggered()
{
    if (GunCovAction->isChecked() == true)
    {
        tdaConfig->setGunCoverageStatus(true);
    }
    else
    {
        tdaConfig->setGunCoverageStatus(false);
    }
    update();
}

void FrameTDA::onGunBarrelActionTriggered()
{
    if (GunBarrelAction->isChecked() == true)
    {
        tdaConfig->setGunBarrelStatus(true);
    }
    else
    {
        tdaConfig->setGunBarrelStatus(false);
    }
    update();
}

void FrameTDA::onZoomChange()
{
    for (int i = 0; i < Z_TOTAL; i++)
    {
        if (ZoomAction[i]->isChecked() && i != cur_checked_zoom_scale)
        {
            ZoomAction[cur_checked_zoom_scale]->setChecked(false);
            cur_checked_zoom_scale = i;
        }
    }
    ZoomAction[cur_checked_zoom_scale]->setChecked(true);
    tdaScale = ZoomAction[cur_checked_zoom_scale]->text().remove(" NM").toDouble();
    tdaConfig->setZoomScale(tdaScale);

    foreach (auto obj, objectItems)
    {
        TDAZoomableObjectBase *objZoom = dynamic_cast<TDAZoomableObjectBase *>(obj);
        if (objZoom)
        {
            objZoom->OnZoom(tdaScale);
        }
    }

    update();
}

void FrameTDA::mouseMoveEvent(QMouseEvent *event)
{
    handleMouseTrackingPolar(event);
    handleMouseTrackinglatLon(event);
}

QString FrameTDA::zoomScale2String(zoomScale scale)
{
    if (scale == Z_025)
        return "0.25 NM";
    else if (scale == Z_050)
        return "0.5 NM";
    else if (scale == Z_010)
        return "1 NM";
    else if (scale == Z_020)
        return "2 NM";
    else if (scale == Z_040)
        return "4 NM";
    else if (scale == Z_080)
        return "8 NM";
    else if (scale == Z_160)
        return "16 NM";
    else if (scale == Z_320)
        return "32 NM";
    else if (scale == Z_640)
        return "64 NM";
    else
        return "0";
}

FrameTDA::zoomScale FrameTDA::zoomString2Scale(QString scale)
{
    if (scale == "0.25")
        return Z_025;
    else if (scale == "0.5")
        return Z_050;
    else if (scale == "1")
        return Z_010;
    else if (scale == "2")
        return Z_020;
    else if (scale == "4")
        return Z_040;
    else if (scale == "8")
        return Z_080;
    else if (scale == "16")
        return Z_160;
    else if (scale == "32")
        return Z_320;
    else if (scale == "64")
        return Z_640;
    else
        return Z_TOTAL;
}

int FrameTDA::zoomScale2Int(zoomScale scale)
{
    if (scale == Z_025)
        return 0;
    else if (scale == Z_050)
        return 1;
    else if (scale == Z_010)
        return 2;
    else if (scale == Z_020)
        return 3;
    else if (scale == Z_040)
        return 4;
    else if (scale == Z_080)
        return 5;
    else if (scale == Z_160)
        return 6;
    else if (scale == Z_320)
        return 7;
    else if (scale == Z_640)
        return 8;
}

FrameTDA::zoomScale FrameTDA::zoomInt2Scale(int scale)
{
    if (scale == 0)
        return Z_025;
    else if (scale == 1)
        return Z_050;
    else if (scale == 2)
        return Z_010;
    else if (scale == 3)
        return Z_020;
    else if (scale == 4)
        return Z_040;
    else if (scale == 5)
        return Z_080;
    else if (scale == 6)
        return Z_160;
    else if (scale == 7)
        return Z_320;
    else if (scale == 8)
        return Z_640;
    else
        return Z_TOTAL;
}

void FrameTDA::setupContextMenu()
{
    QString _zoomScale = QString::number(tdaScale);
    cur_checked_zoom_scale = zoomString2Scale(_zoomScale);

    ZoomSubMenu = new QMenu("Zoom", this);
    ZoomSubMenu->setStyleSheet("background-color: black;");

    for (int i = 0; i < Z_TOTAL; i++)
    {
        ZoomAction[i] = new QAction(zoomScale2String(zoomInt2Scale(i)), this);
        ZoomSubMenu->addAction(ZoomAction[i]);
        ZoomAction[i]->setCheckable(true);
        connect(ZoomAction[i], &QAction::triggered, this, &FrameTDA::onZoomChange);
    }
    ZoomAction[cur_checked_zoom_scale]->setChecked(true);

    CompassAction = new QAction("Show Compass", this);
    HeadingMarkerAction = new QAction("Show Heading Marker", this);
    GunCovAction = new QAction("Show Gun Coverage", this);
    GunBarrelAction = new QAction("Show Gun Barrel", this);

    CompassAction->setCheckable(true);
    HeadingMarkerAction->setCheckable(true);
    GunCovAction->setCheckable(true);
    GunBarrelAction->setCheckable(true);

    if (tdaConfig->getCompassStatus() == true)
        CompassAction->setChecked(true);

    if (tdaConfig->getGunCoverageStatus() == true)
        GunCovAction->setChecked(true);

    if (tdaConfig->getHeadingMarkerStatus() == true)
        HeadingMarkerAction->setChecked(true);

    if (tdaConfig->getGunBarrelStatus() == true)
        GunBarrelAction->setChecked(true);

    connect(this, &FrameTDA::signalOnCostumContextMenuRequest, this, &FrameTDA::on_FrameTDA_customContextMenuRequested);
    connect(CompassAction, &QAction::triggered, this, &FrameTDA::onCompassActionTriggered);
    connect(HeadingMarkerAction, &QAction::triggered, this, &FrameTDA::onHeadingMarkerActionTriggrered);
    connect(GunCovAction, &QAction::triggered, this, &FrameTDA::onGunCovActionTriggered);
    connect(GunBarrelAction, &QAction::triggered, this, &FrameTDA::onGunBarrelActionTriggered);
}

void FrameTDA::setupStatusBar()
{
    statusBarMousePolar = new QStatusBar(this);
    statusBarMousePolar->setObjectName(QString::fromUtf8("statusBarMousePolar"));
    statusBarMouseLatLon = new QStatusBar(this);
    statusBarMouseLatLon->setObjectName(QString::fromUtf8("statusBarMouseLatLon"));
    statusBarSelectedTrack = new QStatusBar(this);
    statusBarSelectedTrack->setObjectName(QString::fromUtf8("statusBarSelectedTrack"));
    statusBarSelectedTrack->hide();
}

void FrameTDA::setupTdaObjects()
{
    TdaCompassObject *compass = new TdaCompassObject(this, tdaConfig);
    TDAGunCoverageObject *gunCoverage = new TDAGunCoverageObject(
                this,
                osdRepo->getRepoOSDInertia(),
                gunRepo->getRepoGunCoverage(),
                tdaConfig);
    TDAHeadingMarkerObject *headingMarker = new TDAHeadingMarkerObject(
                this,
                osdRepo->getRepoOSDInertia(),
                tdaConfig);
    TDAGunBarrelObject *gunBarrel = new TDAGunBarrelObject(
                this,
                osdRepo->getRepoOSDInertia(),
                gunRepo->getRepoGunFeedback(),
                tdaConfig);
    TDATracksObject *tracksObject = new TDATracksObject(
                this,
                trackRepo->getRepoTrackArpa(),
                waService,
                wtaService,
                tdaScale);
    TDAFireTriangleObject *fireTriangle = new TDAFireTriangleObject(
                this,
                fireTriangleRepo->getRepoFT40mm(),
                trackRepo->getRepoTrackArpa(),
                waService,
                wtaService,
                tdaConfig);

    objectItems << compass << tracksObject << headingMarker << gunBarrel << gunCoverage << fireTriangle;
}

void FrameTDA::setupDI()
{
    osdRepo = DI::getInstance()->getRepoOSD();
    gunRepo = DI::getInstance()->getRepoGun();
    tdaConfig = DI::getInstance()->getConfig()->getTDAConfig();
    trackRepo = DI::getInstance()->getRepoTrack();
    fireTriangleRepo = DI::getInstance()->getRepoFireTriangle();
    waService = DI::getInstance()->getServiceWeaponAssign();
    wtaService = DI::getInstance()->getServiceWeaponTrackAssign();

    tdaScale = tdaConfig->getZoomScale();
}

void FrameTDA::setupDateTime()
{
    currentDateTime = QDateTime::currentDateTime();
    QString dateTime = currentDateTime.toString("dd/MM/yyyy hh:mm:ss");
    ui->label_date_time->setText(dateTime);
}

void FrameTDA::handleMouseTrackingPolar(QMouseEvent *event)
{
    QPoint os_pos((width()) / 2, (height() / 2));
    double range_pixel_x = os_pos.x() - event->pos().x();
    double range_pixel_y = os_pos.y() - event->pos().y();
    double bearing = atan2(range_pixel_y, range_pixel_x);
    bearing = (bearing * 180 / M_PI) - 90;
    if (bearing < 0)
        bearing += 360;

    double range = sqrt(pow(range_pixel_y, 2) + pow(range_pixel_x, 2)); // pixel
    range = Utils::pixel2Range(range, tdaScale, width(), height());     // NM
    QString status_mousePolar = QString("Range: %1 NM, Bearing: %2\u00B0").arg(QString::number(range, 'f', 1)).arg(QString::number(bearing, 'f', 1));
    statusBarMousePolar->showMessage(status_mousePolar, 2000);
    QFont font = statusBarMouseLatLon->font();
    QFontMetrics fm(font);
    int width = fm.horizontalAdvance(status_mousePolar);
    statusBarMousePolar->setGeometry(10, height() - 45, width + 20, 20);
}

void FrameTDA::handleMouseTrackinglatLon(QMouseEvent *event)
{
    QPoint os_pos((width()) / 2, (height() / 2));
    const OSDPositionEntity *ownPos = osdRepo->getRepoOSDPosition()->GetPosition();
    QPointF gps = pixToGPS(event->pos().x(), event->pos().y(), width(), height(), tdaScale * 1853., ownPos->latitude(), ownPos->longitude());
    QString status_latlon = QString("Latitude: %1, Longitude: %2").arg(Utils::latDecToStringDegree(gps.y())).arg(Utils::lonDecToStringDegree(gps.x()));
    statusBarMouseLatLon->showMessage(status_latlon, 2000);
    QFont font = statusBarMouseLatLon->font();
    QFontMetrics fm(font);
    int width = fm.horizontalAdvance(status_latlon);
    statusBarMouseLatLon->setGeometry(10, height() - 25, width + 20, 20);
}

QPointF FrameTDA::pixToGPS(const int pos_x, const int pos_y, const int vp_width, const int vp_height, const double vp_range, const double own_lat, const double own_lon)
{
    QPoint screen_middle(vp_width / 2, vp_height / 2);
    double range_pixel_x = screen_middle.x() - pos_x;
    double range_pixel_y = screen_middle.y() - pos_y;
    double angle = atan2(range_pixel_y, range_pixel_x);
    double lat, lon;
    const int MAX_PIX = qMin(vp_width / 2, vp_height / 2);
    double r_mouse_pix = sqrt(pow(range_pixel_y, 2) + pow(range_pixel_x, 2)); // pixel

    angle = (angle * 180 / M_PI) - 90;
    while (angle >= 360. || angle < 0.)
    {
        if (angle >= 360.)
            angle -= 360.;
        if (angle < 0.)
            angle += 360.;
    }

    lat = own_lat +
          static_cast<double>(r_mouse_pix) / static_cast<double>(MAX_PIX) * vp_range * cos(M_PI * angle / 180.) / 60. / 1852.;
    lon = own_lon +
          static_cast<double>(r_mouse_pix) / static_cast<double>(MAX_PIX) * vp_range * sin(M_PI * angle / 180.) /
              cos(M_PI * own_lat / 180.) / 60. / 1852.;

    QPointF pos_to_convert;
    pos_to_convert.setX(lon);
    pos_to_convert.setY(lat);

    return pos_to_convert;
}
