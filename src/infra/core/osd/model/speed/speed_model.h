#ifndef SPEEDMODEL_H
#define SPEEDMODEL_H

#include "src/infra/core/osd/model/osd_base_model.h"
#include <QJsonObject>

class SpeedModel: public OSDBaseModel
{
public:
    SpeedModel(double speed, double course);
    SpeedModel(){}

    static SpeedModel FromJsonObject(QJsonObject obj);

    double getSpeed() const;
    double getCourse() const;

private:
    double Speed, Course;
};

#endif // SPEEDMODEL_H
