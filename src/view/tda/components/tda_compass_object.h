#ifndef TDACOMPASSOBJECT_H
#define TDACOMPASSOBJECT_H

#include <QObject>

#include "tda_object_base.h"
#include "src/shared/config/tda_config.h"

class TdaCompassObject: public TDAObjectBase
{
    Q_OBJECT
public:
    TdaCompassObject(QObject* parent = nullptr, TDAConfig *config = nullptr);
    // bool show_compass; // testCompass
    // TDAConfig *config = nullptr;

private:
    TDAConfig *tdaConfig;

    // TDAObjectBase interface
public:
    void Draw(QPainter *painter, const int &side, const int &width, const int &height, const QPoint &off_center) override;
};

#endif // TDACOMPASSOBJECT_H
