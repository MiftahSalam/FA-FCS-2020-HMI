#ifndef TDATRACKSOBJECT_H
#define TDATRACKSOBJECT_H

#include "src/view/tda/components/tda_object_base.h"

class TDATracksObject : public TDAZoomableObjectBase
{
    Q_OBJECT
public:
    explicit TDATracksObject(QObject *parent = nullptr);

signals:


    // TDAObjectBase interface
public:
    void Draw(QPainter *painter, const int &side, const int &width, const int &height, const QPoint &off_center) override;

    // TDAZoomableObjectBase interface
public slots:
    void OnZoom(float range) override;
};

#endif // TDATRACKSOBJECT_H
