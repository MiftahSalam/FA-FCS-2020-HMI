#ifndef TDAEVENTFILTER_H
#define TDAEVENTFILTER_H

#include <QObject>
#include <QPoint>

class TdaEventFilter : public QObject
{
    Q_OBJECT
public:
    explicit TdaEventFilter(QObject *parent = nullptr);

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

#endif // TDAEVENTFILTER_H
