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
#include "src/shared/config/tda_config.h"
#include "src/view/tda/components/tda_object_base.h"
#include "src/infra/store/gun/gun_repository.h"

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

   TDAConfig *tdaConfig;
//   Configuration *config;

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

   void setupContextMenu();
   void setupStatusBar();
   void setupTdaObjects();
   void setupDI();

   void handleMouseTrackingPolar(QMouseEvent *event);
   void handleMouseTrackinglatLon(QMouseEvent *event);

   QStatusBar *statusBarMousePolar;
   QStatusBar *statusBarMouseLatLon;
   QStatusBar *statusBarSelectedTrack;

   int range2Pixel(double range);
   double pixel2Range(int pixel);
   QPointF pixToGPS(const int pos_x, const int pos_y, const int vp_width, const int vp_height, const double vp_range, const double own_lat, const double own_lon);
};

#endif // FRAMETDA_H
