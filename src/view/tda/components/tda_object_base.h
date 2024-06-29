#ifndef TDA_OBJECT_BASE_H
#define TDA_OBJECT_BASE_H

#include <QObject>
#include <QPainter>

class TDAObjectBase : public QObject
{
    Q_OBJECT
public:
    TDAObjectBase(QObject* parent);

    virtual void Draw(QPainter* painter, const int &side, const int &width, const int &height, const QPoint &off_center) = 0;
};

class TDAZoomableObjectBase : public TDAObjectBase
{
    Q_OBJECT
public:
    TDAZoomableObjectBase(QObject* parent);

public slots:
    virtual void OnZoom(float range) = 0;

private:
    float currentRange;
};

#endif // TDA_OBJECT_BASE_H
