#ifndef TDACOMPASSOBJECT_H
#define TDACOMPASSOBJECT_H

#include <QObject>

#include "tda_object_base.h"

class TdaCompassObject: public TDAObjectBase
{
    Q_OBJECT
public:
    TdaCompassObject(QObject* parent = nullptr);

    // TDAObjectBase interface
public:
    void Draw(QPainter *painter, const int &side, const int &width, const int &height, const QPoint &off_center) override;
};

#endif // TDACOMPASSOBJECT_H
