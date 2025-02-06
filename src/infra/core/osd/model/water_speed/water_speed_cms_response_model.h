#ifndef WATERSpeedResponseModel_H
#define WATERSpeedResponseModel_H

#include "src/infra/core/osd/model/osd_base_model.h"
#include <QJsonObject>

class WaterSpeedResponseModel: public OSDBaseModel
{
public:
    WaterSpeedResponseModel(double w_speed, double w_course);
    WaterSpeedResponseModel(){};

    static WaterSpeedResponseModel fromJsonObject(QJsonObject obj);

    double getSpeed() const;
    double getCourse() const;

private:
    double speed, course;

};

#endif // WATERSpeedResponseModel_H
