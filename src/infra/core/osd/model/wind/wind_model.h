#ifndef WINDMODEL_H
#define WINDMODEL_H

#include "src/infra/core/osd/model/osd_base_model.h"
#include <QJsonObject>

class WindModel: public OSDBaseModel
{
public:
    WindModel(double speed, double direction);
    WindModel(){};

    static WindModel FromJsonObject(QJsonObject obj);

    double getSpeed() const;
    double getDirection() const;

private:
    double speed, direction;
};


#endif // WINDMODEL_H
