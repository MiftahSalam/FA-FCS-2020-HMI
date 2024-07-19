#ifndef TDA_HEADING_MARKER_OBJECT_H
#define TDA_HEADING_MARKER_OBJECT_H

#include <QObject>

#include "tda_object_base.h"

class TDAHeadingMarkerObject: public TDAObjectBase
{
    Q_OBJECT
public:
    TDAHeadingMarkerObject(QObject* parent = nullptr);
    void Draw(QPainter *painter, const int &side, const int &width, const int &height, const QPoint &off_center) override;
};

#endif // TDA_HEADING_MARKER_OBJECT_H
