#ifndef WindResponseModel_H
#define WindResponseModel_H

#include "src/infra/core/osd/model/osd_base_model.h"
#include <QJsonObject>

class WindResponseModel: public OSDBaseModel
{
public:
    WindResponseModel(double speed, double direction);
    WindResponseModel(){};

    static WindResponseModel FromJsonObject(QJsonObject obj);

    double getSpeed() const;
    double getDirection() const;

private:
    double speed, direction;
};


#endif // WindResponseModel_H
