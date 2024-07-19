#ifndef TDA_HEADING_MARKER_H
#define TDA_HEADING_MARKER_H

#include <QObject>
#include "tda_object_base.h"

class TDAHeadingMarker: public TDAObjectBase
{
    Q_OBJECT
public:
    TDAHeadingMarker(QObject* parent = nullptr);

    void Draw(QPainter *painter, const int &side, const int &width, const int &height, const QPoint &off_center) override;

};

#endif // TDA_HEADING_MARKER_H
