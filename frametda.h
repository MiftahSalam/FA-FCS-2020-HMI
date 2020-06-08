#ifndef FRAMETDA_H
#define FRAMETDA_H

#include <redis++.h>
#include <QFrame>
#include <QTimer>
#include <QPainter>
#include <QStatusBar>
#include <QObject>
#include <QPoint>
#include <QSettings>
#include <QMenu>

#include "tda_global.h"
#include "track.h"
#include "rceventhandler.h"


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
    void setHeading(QString heading);

    void setGunbalisticdata(QStringList datagunbalistic);

    void setAccessStatus(QString access_status);

    void setConfig (QString Config);


protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void mousepos(QPointF point);
    void RC_radar(QPoint pos);
    void zoom_change();

   // void selectedTrack();
    void updateDataTracks();

    void track_identity_changed(int tn,Identity identity);

private:

    enum zoomScale
    {
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

   struct tracks
    {
       trackParam trackData;
       track *track_symbol;
    };



   Ui::FrameTDA *ui;

    QTimer *timer;
    QList <int> tnList;
    QMap <int,tracks> *mapTracks;
    QStatusBar *statusBarMouse;
    QStatusBar *statusBarSelectedTrack;
    RCEventHandler *rc_radarevent;
    QAction *zoomAction[Z_TOTAL];
    QMenu *zoomSubMenu;
    QString curStatusString;

    Redis *redisClient;
    QString Config;

    QStringList currentGunBalistic;
    QString currentAccessStatus;
    float currentHeading;
    double tdaScale;

    int cur_track_index_update;
    int cur_checked_zoom_scale;
    int cur_selected_track;


    void loadTrackParam(tracks &bufParam, const trackParam track_data);

    QString zoomScale2String(zoomScale scale);
    zoomScale zoomString2Scale(QString scale);
    int zoomScale2Int(zoomScale scale);
    zoomScale zoomInt2Scale(int scale);
    int range2Pixel(double range);
    double pixel2Range(int pixel);

};

#endif // FRAMETDA_H
