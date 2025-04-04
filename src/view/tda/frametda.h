#ifndef FRAMETDA_H
#define FRAMETDA_H

#include <QFrame>
#include <QList>
#include <QTimer>
#include <QPainter>
#include <QStatusBar>
#include <QObject>
#include <QPoint>
#include <QSettings>
#include <QMenu>
#include <QAction>

#include "src/infra/store/osd/osd_repository.h"
#include "src/infra/store/track/track_repository.h"
#include "src/shared/config/app_config.h"
#include "src/shared/config/tda_config.h"
#include "src/usecase/engagement/weapon_track_assign_service.h"
#include "src/infra/core/osd/stream/osd_stream_datetime.h"
#include "src/usecase/weapon_assign/weapon_assign_service.h"
#include "src/view/tda/components/tda_object_base.h"
#include "src/infra/store/gun/gun_repository.h"
#include "src/infra/store/fire_triangle/fire_triangle_repository.h"

// ==== TDA ==== //
namespace Ui {
class FrameTDA;
}

class FrameTDA : public QFrame
{
    Q_OBJECT
public:
    explicit FrameTDA(QWidget *parent = 0);
    ~FrameTDA();

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private slots:
    void timeOut();
    void on_FrameTDA_customContextMenuRequested(const QPoint &pos);

    void onCompassActionTriggered();
    void onHeadingMarkerActionTriggrered();
    void onGunCovActionTriggered();
    void onGunBarrelActionTriggered();

    void onZoomChange();

    void on_buttonZoomIn_clicked();

    void on_buttonZoomOut_clicked();

signals:
    void signalOnCostumContextMenuRequest(const QPoint &pos);

private:
    Ui::FrameTDA *ui;

    enum zoomScale{
        Z_025 =0,
        Z_050,
        Z_010,
        Z_020,
        Z_040,
        Z_080,
        Z_160,
        Z_320,
        Z_640,
        Z_TOTAL
    };

    double tdaScale;

    QList<TDAObjectBase*> objectItems;
    OSDRepository *osdRepo;
    GunRepository *gunRepo;
    TrackRepository *trackRepo;
    FireTriangleRepository *fireTriangleRepo;
    WeaponAssignService *waService;
    WeaponTrackAssignService *wtaService;

    OSDStreamDateTime *dtStream;
    AppConfig *appConfig;
    TDAConfig *tdaConfig;

    QTimer *timer;
    QMenu *ZoomSubMenu;
    QAction *CompassAction;
    QAction *HeadingMarkerAction;
    QAction *GunCovAction;
    QAction *GunBarrelAction;
    QAction *ZoomAction[Z_TOTAL];

    QString zoomScale2String(zoomScale scale);
    zoomScale zoomString2Scale(QString scale);
    int zoomScale2Int(zoomScale scale);
    zoomScale zoomInt2Scale(int scale);
    int cur_checked_zoom_scale;
    QDateTime currentDateTime;

    void setupContextMenu();
    void setupStatusBar();
    void setupTdaObjects();
    void setupDI();
    void updateDateTime();

    void handleMouseTrackingPolar(QMouseEvent *event);
    void handleMouseTrackinglatLon(QMouseEvent *event);

    QStatusBar *statusBarMousePolar;
    QStatusBar *statusBarMouseLatLon;
    QStatusBar *statusBarSelectedTrack;

    QPointF pixToGPS(const int pos_x, const int pos_y, const int vp_width, const int vp_height, const double vp_range, const double own_lat, const double own_lon);

    QString dateTimeLocal, dateTimeUTC;
};

#endif // FRAMETDA_H
