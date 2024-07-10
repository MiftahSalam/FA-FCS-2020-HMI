#ifndef WATERSPEEDMODEL_H
#define WATERSPEEDMODEL_H

#include <QJsonObject>

class WaterSpeedModel
{
public:
    WaterSpeedModel(double w_speed, double w_course);
    WaterSpeedModel(){};

    static WaterSpeedModel fromJsonObject(QJsonObject obj);

    double getW_Speed() const;
    double getW_Course() const;

private:
    double waterspeed, watercourse;

};

#endif // WATERSPEEDMODEL_H
