#ifndef SpeedResponseModel_H
#define SpeedResponseModel_H

#include "src/infra/core/osd/model/osd_base_model.h"
#include <QJsonObject>

class SpeedResponseModel: public OSDBaseModel
{
public:
    SpeedResponseModel(double speed, double course);
    SpeedResponseModel(){}

    static SpeedResponseModel FromJsonObject(QJsonObject obj);

    double getSpeed() const;
    double getCourse() const;

private:
    double Speed, Course;
};

#endif // SpeedResponseModel_H
