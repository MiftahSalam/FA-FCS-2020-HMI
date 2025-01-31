#ifndef POSITIONMODEL_H
#define POSITIONMODEL_H

#include "src/infra/core/osd/model/osd_base_model.h"
#include <QJsonObject>

class PositionModel: public OSDBaseModel
{
public:
    PositionModel(double lat, double lon);
    PositionModel(){};

    static PositionModel fromJsonObject(QJsonObject obj);

    double getLatitude() const;
    double getLongitude() const;

private:
    double latitude, longitude;

};

#endif // POSITIONMODEL_H
