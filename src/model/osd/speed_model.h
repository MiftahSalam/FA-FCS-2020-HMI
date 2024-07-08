#ifndef SPEEDMODEL_H
#define SPEEDMODEL_H

#include <QJsonObject>

class SpeedModel
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
