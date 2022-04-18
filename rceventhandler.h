#ifndef RCEVENTHANDLER_H
#define RCEVENTHANDLER_H

#include <QObject>
#include <QPoint>

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
    void tooltip(const QPoint pos);
};

#endif // RCEVENTHANDLER_H
