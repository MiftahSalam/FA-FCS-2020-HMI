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

private slots:
    void timeOut();
    void on_FrameTDA_customContextMenuRequested(const QPoint &pos);

    void onCompassActionTriggered();
    void onHeadingMarkerActionTriggrered();
    void onGunCovActionTriggered();
    void onGunBarrelActionTriggered();

signals:
    void signalOnCostumContextMenuRequest(const QPoint &pos);


private:
   Ui::FrameTDA *ui;

   QList<TDAObjectBase*> objectItems;
   OSDRepository *osdRepo;
   GunRepository *gunRepo;

   QTimer *timer;
   QMenu *ZoomSubMenu;
   QAction *CompassAction;
   QAction *HeadingMarkerAction;
   QAction *GunCovAction;
   QAction *GunBarrelAction;
};

#endif // FRAMETDA_H
