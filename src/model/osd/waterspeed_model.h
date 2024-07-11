#ifndef WATERSPEEDMODEL_H
#define WATERSPEEDMODEL_H

#include <QJsonObject>

class WaterSpeedModel
{
public:
    WaterSpeedModel(double w_speed, double w_course);
    WaterSpeedModel(){};

    static WaterSpeedModel fromJsonObject(QJsonObject obj);

    double getSpeed() const;
    double getCourse() const;

private:
    double speed, course;

};

#endif // WATERSPEEDMODEL_H
