#ifndef SPEEDMODEL_H
#define SPEEDMODEL_H

#include <QJsonObject>

class SpeedModel
{
public:
    SpeedModel(double sog, double cog);

    static SpeedModel fromJsonObject(QJsonObject obj);

    double getSpeed_OG() const;
    double getCourse_OG() const;

private:
    double speed, course;

};

#endif // SPEEDMODEL_H
