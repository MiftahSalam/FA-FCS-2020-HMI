#ifndef PositionResponseModel_H
#define PositionResponseModel_H

#include "src/infra/core/osd/model/osd_base_model.h"
#include <QJsonObject>

class PositionResponseModel: public OSDBaseModel
{
public:
    PositionResponseModel(double lat, double lon);
    PositionResponseModel(){};

    static PositionResponseModel fromJsonObject(QJsonObject obj);

    double getLatitude() const;
    double getLongitude() const;

private:
    double latitude, longitude;

};

#endif // PositionResponseModel_H
