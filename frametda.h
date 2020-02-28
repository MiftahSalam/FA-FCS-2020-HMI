#ifndef FRAMETDA_H
#define FRAMETDA_H

#include <QFrame>
#include <QTimer>
#include <QPainter>
#include <QStatusBar>
#include <QObject>
#include <QPoint>
#include <QSettings>
#include <QMenu>

// ==== RCEventHandler ==== //
class RCEventHandler : public QObject
{
Q_OBJECT
public:
    explicit RCEventHandler(QObject *parent = 0) : QObject(parent) {}

protected:
    bool eventFilter(QObject *obj, QEvent *event);

signals:
    void send_rightButtonClicked(const QPoint &p);
    void send_leftButtonClicked(const QPointF &p);
    void send_leftButtonReleased(const QPointF &p);
    void move_mouse(const QPointF &p);
    void hover_enter(); //30 jan
    void hover_leave();
};

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

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void mousepos(QPointF point);
    void RC_radar(QPoint pos);
    void zoom_change();

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

    Ui::FrameTDA *ui;

    QTimer *timer;
    QStatusBar *statusBarMouse;
    QStatusBar *statusBarSelectedTrack;
    RCEventHandler *rc_radarevent;
    QAction *zoomAction[Z_TOTAL];
    QMenu *zoomSubMenu;

    float currentHeading;
    double tdaScale;

    int cur_track_index_update;
    int cur_checked_zoom_scale;
    int cur_selected_track;

    QString zoomScale2String(zoomScale scale);
    zoomScale zoomString2Scale(QString scale);
    int zoomScale2Int(zoomScale scale);
    zoomScale zoomInt2Scale(int scale);
    int range2Pixel(double range);
    double pixel2Range(int pixel);

};

#endif // FRAMETDA_H
